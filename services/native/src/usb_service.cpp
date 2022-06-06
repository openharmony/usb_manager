/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "usb_service.h"
#include <cstdio>
#include <iostream>
#include <ipc_skeleton.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include "bundle_mgr_interface.h"
#include "bundle_mgr_proxy.h"
#include "file_ex.h"
#include "if_system_ability_manager.h"
#include "iservice_registry.h"
#include "iusb_srv.h"
#include "securec.h"
#include "system_ability_definition.h"
#include "usb_common.h"
#include "usb_descriptor_parser.h"
#include "usb_errors.h"
#include "usb_port_manager.h"
#include "usb_right_manager.h"

namespace OHOS {
namespace USB {
namespace {
constexpr const char *USB_SERVICE_NAME = "UsbService";
constexpr int32_t COMMEVENT_REGISTER_RETRY_TIMES = 10;
constexpr int32_t COMMEVENT_REGISTER_WAIT_DELAY_US = 20000;
constexpr uint32_t CURSOR_INIT = 18;
constexpr int32_t DESCRIPTOR_TYPE_STRING = 3;
constexpr int32_t DESCRIPTOR_VALUE_START_OFFSET = 2;
constexpr int32_t HALF = 2;
constexpr int32_t BIT_SHIFT_4 = 4;
constexpr int32_t BIT_HIGH_4 = 0xF0;
constexpr int32_t BIT_LOW_4 = 0x0F;
constexpr int32_t SERVICE_STARTUP_MAX_TIME = 30;
} // namespace

auto pms = DelayedSpSingleton<UsbService>::GetInstance();
const bool G_REGISTER_RESULT =
    SystemAbility::MakeAndRegisterAbility(DelayedSpSingleton<UsbService>::GetInstance().GetRefPtr());

UsbService::UsbService() : SystemAbility(USB_SYSTEM_ABILITY_ID, true)
{
    usbHostManger_ = std::make_shared<UsbHostManager>(nullptr);
    usbRightManager_ = std::make_shared<UsbRightManager>();
    usbPortManager_ = std::make_shared<UsbPortManager>();
    usbDeviceManager_ = std::make_shared<UsbDeviceManager>();
}
UsbService::~UsbService() {}

void UsbService::OnStart()
{
    USB_HILOGI(MODULE_USB_SERVICE, "usb_service OnStart enter");
    if (ready_) {
        USB_HILOGE(MODULE_USB_SERVICE, "OnStart is ready, nothing to do");
        return;
    }

    if (!(Init())) {
        USB_HILOGE(MODULE_USB_SERVICE, "OnStart call init fail");
        return;
    }
    
    // wait for the usbd service to start and bind usb service and usbd service
    int32_t retryTimes = 0;
    while (retryTimes < SERVICE_STARTUP_MAX_TIME) {
        if (InitUsbd() != 0) {
            break;
        }
        sleep(1);
        retryTimes++;
        
        if (retryTimes == SERVICE_STARTUP_MAX_TIME) {
            USB_HILOGE(MODULE_USB_SERVICE, "OnStart call initUsbd fail");
            return;
        }
    }

    if (usbPortManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbPortManager_");
        return;
    }
    
    usbPortManager_->Init();
    ready_ = true;
    USB_HILOGE(MODULE_USB_SERVICE, "OnStart and add system ability success");
}

bool UsbService::Init()
{
    USB_HILOGI(MODULE_USB_SERVICE, "usb_service Init enter");
    if (!eventRunner_) {
        eventRunner_ = AppExecFwk::EventRunner::Create(USB_SERVICE_NAME);
        if (eventRunner_ == nullptr) {
            USB_HILOGE(MODULE_USB_SERVICE, "Init failed due to create EventRunner");
            return false;
        }
    }
    if (handler_ == nullptr) {
        handler_ = std::make_shared<UsbServerEventHandler>(eventRunner_, pms);

        if (!Publish(pms)) {
            USB_HILOGE(MODULE_USB_SERVICE, "OnStart register to system ability manager failed.");
            return false;
        }
    }
    while (commEventRetryTimes_ <= COMMEVENT_REGISTER_RETRY_TIMES) {
        if (!IsCommonEventServiceAbilityExist()) {
            ++commEventRetryTimes_;
            usleep(COMMEVENT_REGISTER_WAIT_DELAY_US);
        } else {
            commEventRetryTimes_ = 0;
            break;
        }
    }
    USB_HILOGE(MODULE_USB_SERVICE, "Init success");
    return true;
}

bool UsbService::InitUsbd()
{
    usbdSubscriber_ = new(std::nothrow) UsbServiceSubscriber();
    if (usbdSubscriber_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "Init failed\n");
        return false;
    }

    ErrCode ret = UsbdClient::GetInstance().BindUsbdSubscriber(usbdSubscriber_);
    USB_HILOGI(MODULE_USB_SERVICE, "entry InitUsbd ret: %{public}d", ret);
    return SUCCEEDED(ret);
}

void UsbService::OnStop()
{
    USB_HILOGI(MODULE_USB_SERVICE, "entry stop service %{public}d", ready_);
    if (!ready_) {
        return;
    }
    eventRunner_.reset();
    handler_.reset();
    ready_ = false;
    UsbdClient::GetInstance().UnbindUsbdSubscriber(usbdSubscriber_);
}

bool UsbService::IsCommonEventServiceAbilityExist()
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sm == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE,
                   "IsCommonEventServiceAbilityExist Get ISystemAbilityManager "
                   "failed, no SystemAbilityManager");
        return false;
    }
    sptr<IRemoteObject> remote = sm->CheckSystemAbility(COMMON_EVENT_SERVICE_ID);
    if (!remote) {
        USB_HILOGE(MODULE_USB_SERVICE, "No CesServiceAbility");
        return false;
    }
    return true;
}

int32_t UsbService::OpenDevice(uint8_t busNum, uint8_t devAddr)
{
    std::string name = std::to_string(busNum) + "-" + std::to_string(devAddr);
    if (!UsbService::HasRight(name)) {
        USB_HILOGE(MODULE_USB_SERVICE, "No permission");
        return UEC_SERVICE_PERMISSION_DENIED;
    }

    const UsbDev dev = {busNum, devAddr};
    int32_t ret = UsbdClient::GetInstance().OpenDevice(dev);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "OpenDevice failed ret:%{public}d", ret);
    }

    return ret;
}

bool UsbService::HasRight(std::string deviceName)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbRightManager HasRight");
    std::string bundleName;
    if (!GetBundleName(bundleName)) {
        USB_HILOGE(MODULE_USB_SERVICE, "HasRight GetBundleName false");
        return false;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "HasRight bundleName = %{public}s", bundleName.c_str());

    if (usbRightManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbRightManager_");
        return false;
    }
    return usbRightManager_->HasRight(deviceName, bundleName);
}

int32_t UsbService::RequestRight(std::string deviceName)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbRightManager RequestRight");
    std::string bundleName;
    if (!GetBundleName(bundleName)) {
        USB_HILOGI(MODULE_USB_SERVICE, "RequestRight GetBundleName false");
        return UEC_SERVICE_INNER_ERR;
    }

    if (usbRightManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbRightManager_");
        return UEC_SERVICE_INVALID_VALUE;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "RequestRight bundleName = %{public}s", bundleName.c_str());
    return usbRightManager_->RequestRight(deviceName, bundleName);
}

int32_t UsbService::RemoveRight(std::string deviceName)
{
    if (usbRightManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbRightManager_");
        return UEC_SERVICE_INVALID_VALUE;
    }

    if (usbRightManager_->RemoveDeviceRight(deviceName)) {
        return UEC_OK;
    }
    return UEC_SERVICE_INNER_ERR;
}

int32_t UsbService::GetDevices(std::vector<UsbDevice> &deviceList)
{
    std::map<std::string, UsbDevice *> devices;

    if (usbHostManger_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbHostManger_");
        return UEC_SERVICE_INVALID_VALUE;
    }

    usbHostManger_->GetDevices(devices);
    USB_HILOGI(MODULE_USB_SERVICE, "list size %{public}zu", devices.size());
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        deviceList.push_back(*it->second);
    }
    return UEC_OK;
}

int32_t UsbService::GetCurrentFunctions(int32_t &functions)
{
    return UsbdClient::GetInstance().GetCurrentFunctions(functions);
}

int32_t UsbService::SetCurrentFunctions(int32_t functions)
{
    USB_HILOGI(MODULE_USB_SERVICE, "func = %{public}d", functions);
    if (usbDeviceManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbDeviceManager_");
        return UEC_SERVICE_INVALID_VALUE;
    }

    usbDeviceManager_->UpdateFunctions(functions);
    return UsbdClient::GetInstance().SetCurrentFunctions(functions);
}

int32_t UsbService::UsbFunctionsFromString(std::string_view funcs)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling UsbFunctionsFromString");
    return UsbDeviceManager::ConvertFromString(funcs);
}

std::string UsbService::UsbFunctionsToString(int32_t funcs)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling UsbFunctionsToString");
    return UsbDeviceManager::ConvertToString(funcs);
}

int32_t UsbService::GetPorts(std::vector<UsbPort> &ports)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbPortManager getPorts");
    if (usbPortManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbPortManager_");
        return UEC_SERVICE_INVALID_VALUE;
    }
    return usbPortManager_->GetPorts(ports);
}

int32_t UsbService::GetSupportedModes(int32_t portId, int32_t &supportedModes)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbPortManager getSupportedModes");
    if (usbPortManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbPortManager_");
        return UEC_SERVICE_INVALID_VALUE;
    }
    return usbPortManager_->GetSupportedModes(portId, supportedModes);
}

int32_t UsbService::SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbd getPorts");
    return UsbdClient::GetInstance().SetPortRole(portId, powerRole, dataRole);
}

int32_t UsbService::ClaimInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface, uint8_t force)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().ClaimInterface(dev, interface, force);
}

int32_t UsbService::ReleaseInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().ReleaseInterface(dev, interface);
}

int32_t UsbService::BulkTransferRead(const UsbDev &devInfo, const UsbPipe &pipe, std::vector<uint8_t> &bufferData,
    int32_t timeOut)
{
    int32_t ret = UsbdClient::GetInstance().BulkTransferRead(devInfo, pipe, timeOut, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkTransferRead error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &bufferData,
    int32_t timeOut)
{
    int32_t ret = UsbdClient::GetInstance().BulkTransferWrite(dev, pipe, timeOut, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkTransferWrite error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &bufferData)
{
    int32_t ret = UsbdClient::GetInstance().ControlTransfer(dev, ctrl, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::SetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t configIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().SetConfig(dev, configIndex);
}

int32_t UsbService::GetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t &configIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().GetConfig(dev, configIndex);
}

int32_t UsbService::SetInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t altIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().SetInterface(dev, interfaceid, altIndex);
}

int32_t UsbService::GetRawDescriptor(uint8_t busNum, uint8_t devAddr, std::vector<uint8_t> &bufferData)
{
    const UsbDev dev = {busNum, devAddr};
    int32_t ret = UsbdClient::GetInstance().GetRawDescriptor(dev, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::GetFileDescriptor(uint8_t busNum, uint8_t devAddr, int32_t &fd)
{
    const UsbDev dev = {busNum, devAddr};
    int32_t ret = UsbdClient::GetInstance().GetFileDescriptor(dev, fd);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::RequestQueue(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &clientData,
    const std::vector<uint8_t> &bufferData)
{
    int32_t ret = UsbdClient::GetInstance().RequestQueue(dev, pipe, clientData, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::RequestWait(const UsbDev &dev, int32_t timeOut, std::vector<uint8_t> &clientData,
    std::vector<uint8_t> &bufferData)
{
    int32_t ret = UsbdClient::GetInstance().RequestWait(dev, clientData, bufferData, timeOut);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::RequestCancel(uint8_t busNum, uint8_t devAddr, uint8_t interfaceId, uint8_t endpointId)
{
    const UsbDev dev = {busNum, devAddr};
    const UsbPipe pipe = {interfaceId, endpointId};
    return UsbdClient::GetInstance().RequestCancel(dev, pipe);
}

int32_t UsbService::Close(uint8_t busNum, uint8_t devAddr)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetInstance().CloseDevice(dev);
}

static std::string GetDevStringValFromIdx(uint8_t busNum, uint8_t devAddr, uint8_t idx)
{
    const UsbDev dev = {busNum, devAddr};
    std::vector<uint8_t> strV;
    std::string strDesc = " ";

    if (idx == 0) {
        return strDesc;
    }

    int32_t ret = UsbdClient::GetInstance().GetStringDescriptor(dev, idx, strV);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "get string[%{public}hhu] failed ret:%{public}d", idx, ret);
        return strDesc;
    }
    uint32_t length = strV.size();
    if ((length < DESCRIPTOR_VALUE_START_OFFSET) || (strV[1] != DESCRIPTOR_TYPE_STRING)) {
        USB_HILOGI(MODULE_USB_SERVICE, "type or length error, len:%{public}u", length);
        return strDesc;
    }

    uint16_t *tbuf = new(std::nothrow) uint16_t[length + 1]();
    if (tbuf == nullptr) {
        USB_HILOGI(MODULE_USB_SERVICE, "new failed\n");
        return strDesc;
    }

    for (uint32_t i = 0; i < length - DESCRIPTOR_VALUE_START_OFFSET; ++i) {
        tbuf[i] = strV[i + DESCRIPTOR_VALUE_START_OFFSET];
    }
    std::wstring wstr((wchar_t *)(tbuf), (length - DESCRIPTOR_VALUE_START_OFFSET) / HALF);
    strDesc = std::string(wstr.begin(), wstr.end());
    USB_HILOGI(MODULE_USB_SERVICE, "getString idx:%{public}d String:%{public}s length:%{public}d", idx, strDesc.c_str(),
        length);
    delete[] tbuf;
    return strDesc;
}

static std::string BcdToString(const std::vector<uint8_t> &bcd)
{
    std::string tstr;
    for (uint32_t i = 0; i < bcd.size(); ++i) {
        tstr += std::to_string((bcd[i] & BIT_HIGH_4) >> BIT_SHIFT_4);
        tstr += std::to_string((bcd[i] & BIT_LOW_4));
    }
    return tstr;
}

static int32_t FillDevStrings(UsbDevice &dev)
{
    uint8_t busNum;
    uint8_t devAddr;
    uint8_t offsetValue = 8;

    busNum = dev.GetBusNum();
    devAddr = dev.GetDevAddr();
    uint16_t bcdDevice = dev.GetbcdDevice();
    const std::vector<uint8_t> bcdData {(bcdDevice & 0xff), ((bcdDevice >> offsetValue) & 0xff)};
    dev.SetVersion(BcdToString(bcdData));
    dev.SetManufacturerName(GetDevStringValFromIdx(busNum, devAddr, dev.GetiManufacturer()));
    dev.SetProductName(GetDevStringValFromIdx(busNum, devAddr, dev.GetiProduct()));
    dev.SetmSerial(GetDevStringValFromIdx(busNum, devAddr, dev.GetiSerialNumber()));
    USB_HILOGI(MODULE_USB_SERVICE, "iSerial:%{public}d mSerial:%{public}s Manufactur:%{public}s product:%{public}s "
        "version:%{public}s", dev.GetiSerialNumber(), dev.GetmSerial().c_str(), dev.GetManufacturerName().c_str(),
        dev.GetProductName().c_str(), dev.GetVersion().c_str());

    std::vector<USBConfig> configs;
    configs = dev.GetConfigs();
    for (auto it = configs.begin(); it != configs.end(); ++it) {
        it->SetName(GetDevStringValFromIdx(busNum, devAddr, it->GetiConfiguration()));
        USB_HILOGI(MODULE_USB_SERVICE, "Config:%{public}d %{public}s", it->GetiConfiguration(), it->GetName().c_str());
        std::vector<UsbInterface> interfaces = it->GetInterfaces();
        for (auto itIF = interfaces.begin(); itIF != interfaces.end(); ++itIF) {
            itIF->SetName(GetDevStringValFromIdx(busNum, devAddr, itIF->GetiInterface()));
            USB_HILOGI(MODULE_USB_SERVICE, "interface:%{public}hhu %{public}s", itIF->GetiInterface(),
                itIF->GetName().c_str());
        }
        it->SetInterfaces(interfaces);
    }
    dev.SetConfigs(configs);

    return UEC_OK;
}

int32_t UsbService::GetDeviceInfoDescriptor(const UsbDev &uDev, std::vector<uint8_t> &descriptor, UsbDevice &dev)
{
    int32_t ret = UsbdClient::GetInstance().GetRawDescriptor(uDev, descriptor);
    if (ret != UEC_OK) {
        UsbdClient::GetInstance().CloseDevice(uDev);
        USB_HILOGE(MODULE_USB_SERVICE, "GetRawDescriptor failed ret=%{public}d busNum:%{public}d devAddr:%{public}d",
            ret, uDev.busNum, uDev.devAddr);
        return ret;
    }
    uint8_t *buffer = descriptor.data();
    uint32_t length = descriptor.size();
    if ((!buffer) || (length == 0)) {
        USB_HILOGE(MODULE_USB_SERVICE, "GetRawDescriptor failed len=%{public}d busNum:%{public}d devAddr:%{public}d",
            length, uDev.busNum, uDev.devAddr);
        return UEC_SERVICE_INVALID_VALUE;
    }
    dev.SetBusNum(uDev.busNum);
    dev.SetDevAddr(uDev.devAddr);
    dev.SetName(std::to_string(uDev.busNum) + "-" + std::to_string(uDev.devAddr));

    ret = UsbDescriptorParser::ParseDeviceDescriptor(buffer, length, dev);
    if (ret != UEC_OK) {
        UsbdClient::GetInstance().CloseDevice(uDev);
        USB_HILOGE(MODULE_USB_SERVICE, "ParseDeviceDescriptor failed ret=%{public}d", ret);
        return ret;
    }
    return ret;
}

int32_t UsbService::GetConfigDescriptor(UsbDevice &dev, std::vector<uint8_t> &descriptor)
{
    std::vector<USBConfig> configs;
    uint8_t *buffer = descriptor.data();
    uint32_t length = descriptor.size();
    uint32_t cursor = CURSOR_INIT;
    int32_t ret = UEC_OK;
    for (uint8_t i = 0; i < dev.GetDescConfigCount(); ++i) {
        if (length <= cursor) {
            USB_HILOGE(MODULE_USB_SERVICE, "GetConfigDescriptor[%{public}d] length=%{public}d", i, length);
            break;
        }
        USB_HILOGI(MODULE_USB_SERVICE, "GetConfigDescriptor length=%{public}d", length);
        uint32_t configCursor = 0;
        USBConfig config;
        ret = UsbDescriptorParser::ParseConfigDescriptor(buffer + cursor, length - cursor, configCursor, config);
        if (ret != UEC_OK) {
            USB_HILOGE(MODULE_USB_SERVICE, "ParseConfigDescriptor failed ret=%{public}d", ret);
            return ret;
        }
        cursor += configCursor;
        configs.push_back(config);
    }
    dev.SetConfigs(configs);
    ret = FillDevStrings(dev);
    USB_HILOGI(MODULE_USB_SERVICE, "FillDevStrings ret=%{public}d", ret);
    return ret;
}

int32_t UsbService::GetDeviceInfo(uint8_t busNum, uint8_t devAddr, UsbDevice &dev)
{
    USB_HILOGI(MODULE_USB_SERVICE, "busNum:%{public}d devAddr:%{public}d", busNum, devAddr);
    const UsbDev uDev = {busNum, devAddr};
    std::vector<uint8_t> descriptor;

    int32_t ret = UsbdClient::GetInstance().OpenDevice(uDev);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "OpenDevice failed ret=%{public}d", ret);
        return ret;
    }

    ret = GetDeviceInfoDescriptor(uDev, descriptor, dev);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "GetDeviceInfoDescriptor ret=%{public}d", ret);
    }
    ret = GetConfigDescriptor(dev, descriptor);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "GetConfigDescriptor ret=%{public}d", ret);
        return ret;
    }

    UsbdClient::GetInstance().CloseDevice(uDev);
    USB_HILOGI(MODULE_USB_SERVICE, "CloseDevice=%{public}s", dev.ToString().c_str());

    return UEC_OK;
}

bool UsbService::AddDevice(uint8_t busNum, uint8_t devAddr)
{
    UsbDevice *devInfo = new(std::nothrow) UsbDevice();
    if (devInfo == nullptr) {
        USB_HILOGI(MODULE_USB_SERVICE, "new failed");
        return false;
    }

    errno_t retSafe = memset_s(devInfo, sizeof(UsbDevice), 0, sizeof(UsbDevice));
    if (retSafe != EOK) {
        USB_HILOGI(MODULE_USB_SERVICE, "memset_s failed");
        return false;
    }

    int32_t ret = GetDeviceInfo(busNum, devAddr, *devInfo);
    USB_HILOGI(MODULE_USB_SERVICE, "GetDeviceInfo ret=%{public}d", ret);
    if (ret != UEC_OK) {
        delete devInfo;
        return false;
    }

    if (usbHostManger_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbHostManger_");
        return false;
    }

    usbHostManger_->AddDevice(devInfo);
    return true;
}

bool UsbService::DelDevice(uint8_t busNum, uint8_t devAddr)
{
    USB_HILOGI(MODULE_USBD, "entry");
    int32_t ret = Close(busNum, devAddr);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USBD, "Close device failed width ret = %{public}d", ret);
    }

    if (usbHostManger_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbHostManger_");
        return false;
    }

    return usbHostManger_->DelDevice(busNum, devAddr);
}

void UsbService::UpdateUsbPort(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode)
{
    if (usbPortManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbPortManager_");
        return;
    }

    usbPortManager_->UpdatePort(portId, powerRole, dataRole, mode);
}

void UsbService::UpdateDeviceState(int32_t status)
{
    if (usbDeviceManager_ == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "invalid usbDeviceManager_");
        return;
    }

    usbDeviceManager_->HandleEvent(status);
}

bool UsbService::GetBundleName(std::string &bundleName)
{
#ifdef USB_RIGHT_TEST
    bundleName = "com.usb.right";
    return true;
#endif
    pid_t uid = GetCallingUid();
    sptr<ISystemAbilityManager> systemAbilityManager =
        SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityManager == nullptr) {
        return false;
    }
    sptr<IRemoteObject> remoteObject = systemAbilityManager->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
    if (remoteObject == nullptr) {
        return false;
    }

    sptr<AppExecFwk::IBundleMgr> bundleMgr(new AppExecFwk::BundleMgrProxy(remoteObject));
    if (bundleMgr == nullptr) {
        return false;
    }
    bundleMgr->GetBundleNameForUid(uid, bundleName);
    return true;
}

int32_t UsbService::RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const sptr<IRemoteObject> &cb)
{
    int32_t ret = UsbdClient::GetInstance().RegBulkCallback(devInfo, pipe, cb);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "RegBulkCallback error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe)
{
    int32_t ret = UsbdClient::GetInstance().UnRegBulkCallback(devInfo, pipe);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "UnRegBulkCallback error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
{
    if (ashmem == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkRead error ashmem");
        return UEC_SERVICE_INVALID_VALUE;
    }
    int32_t ret = UsbdClient::GetInstance().BulkRead(devInfo, pipe, ashmem);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkRead error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
{
    if (ashmem == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkWrite error ashmem");
        return UEC_SERVICE_INVALID_VALUE;
    }
    int32_t ret = UsbdClient::GetInstance().BulkWrite(devInfo, pipe, ashmem);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkWrite error ret:%{public}d", ret);
    }
    return ret;
}

int32_t UsbService::BulkCancel(const UsbDev &devInfo, const UsbPipe &pipe)
{
    int32_t ret = UsbdClient::GetInstance().BulkCancel(devInfo, pipe);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "BulkCancel error ret:%{public}d", ret);
    }
    return ret;
}
} // namespace USB
} // namespace OHOS
