/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#include "file_ex.h"
#include "if_system_ability_manager.h"
#include "iservice_registry.h"
#include "iusb_srv.h"
#include "securec.h"
#include "system_ability_definition.h"
#include "usb_common.h"
#include "usb_descriptor_parser.h"
#include "usb_port_manager.h"
#include "usb_right_manager.h"
#include "bundle_mgr_proxy.h"

namespace OHOS {
namespace USB {
namespace {
const std::string USB_SERVICE_NAME = "UsbService";
constexpr int32_t COMMEVENT_REGISTER_RETRY_TIMES = 10;
constexpr int32_t COMMEVENT_REGISTER_WAIT_DELAY_US = 20000;
const int32_t USB_MAX_DESCRIPTOR_SIZE = 256;
const int32_t DESCRIPTOR_TYPE_STRING = 3;
const int32_t DESCRIPTOR_VALUE_START_OFFSET = 2;
const int32_t HALF = 2;
const int32_t BIT_SHIFT_4 = 4;
const int32_t BIT_HIGH_4 = 0xF0;
const int32_t BIT_LOW_4 = 0x0F;
} // namespace

auto pms = DelayedSpSingleton<UsbService>::GetInstance();
const bool G_REGISTER_RESULT =
    SystemAbility::MakeAndRegisterAbility(DelayedSpSingleton<UsbService>::GetInstance().GetRefPtr());

UsbService::UsbService() : SystemAbility(USB_MANAGER_USB_SERVICE_ID, true)
{
    usbHostManger_ = new UsbHostManager(nullptr);
    usbRightManager = new UsbRightManager();
    usbPortManager = new UsbPortManager();
    usbFunctionManager = new UsbFunctionManager();
}
UsbService::~UsbService()
{
    delete usbHostManger_;
    delete usbRightManager;
    delete usbPortManager;
}

void UsbService::OnStart()
{
    USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d usb_service OnStart enter", __func__, __LINE__);
    if (ready_) {
        USB_HILOGE(MODULE_USB_SERVICE, "OnStart is ready, nothing to do");
        return;
    }
    if (!(Init())) {
        USB_HILOGE(MODULE_USB_SERVICE, "OnStart call init fail");
        return;
    }
    if (!(InitUsbd())) {
        USB_HILOGE(MODULE_USB_SERVICE, "OnStart call initUsbd fail");
        return;
    }
    usbPortManager->Init();
    ready_ = true;
    USB_HILOGE(MODULE_USB_SERVICE, "OnStart and add system ability success");
}

bool UsbService::Init()
{
    USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d usb_service Init enter", __func__, __LINE__);
    if (!eventRunner_) {
        eventRunner_ = AppExecFwk::EventRunner::Create(USB_SERVICE_NAME);
        if (eventRunner_ == nullptr) {
            USB_HILOGE(MODULE_USB_SERVICE, "Init failed due to create EventRunner");
            return false;
        }
    }
    if (!handler_) {
        handler_ = std::make_shared<UsbServerEventHandler>(eventRunner_, pms);
        if (handler_ == nullptr) {
            USB_HILOGE(MODULE_USB_SERVICE, "Init failed due to create handler error");
            return false;
        }
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
    usbdSubscriber_ = new UsbServiceSubscriber();
    ErrCode ret = UsbdClient::BindUsbdSubscriber(usbdSubscriber_);
    USB_HILOGE(MODULE_USB_SERVICE, " entry InitUsbd ret: %{public}d", ret);
    return SUCCEEDED(ret);
}

void UsbService::OnStop()
{
    USB_HILOGE(MODULE_USB_SERVICE, " entry stop service %{public}d", ready_);
    if (!ready_) {
        return;
    }
    eventRunner_.reset();
    handler_.reset();
    ready_ = false;
    UsbdClient::UnbindUsbdSubscriber();
}

bool UsbService::IsCommonEventServiceAbilityExist()
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (!sm) {
        USB_HILOGE(MODULE_USB_SERVICE,
                   "IsCommonEventServiceAbilityExist Get ISystemAbilityManager "
                   "failed, no SystemAbilityManager");
        return false;
    }
    sptr<IRemoteObject> remote = sm->CheckSystemAbility(COMMON_EVENT_SERVICE_ABILITY_ID);
    if (!remote) {
        USB_HILOGE(MODULE_USB_SERVICE, "No CesServiceAbility");
        return false;
    }
    return true;
}
int32_t UsbService::OpenDevice(uint8_t busNum, uint8_t devAddr)
{
    std::string name = std::to_string(busNum) + "-" + std::to_string(devAddr);
    int32_t ret = UsbService::HasRight(name);
    if (UEC_OK == ret) {
        const UsbDev dev = {busNum, devAddr};
        ret = UsbdClient::OpenDevice(dev);
        if (UEC_OK != ret) {
            USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d OpenDevice failed ret:%{public}d", __func__, __LINE__,
                       ret);
        }
    } else {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d No permission ret:%{public}d", __func__, __LINE__, ret);
        ret = UEC_SERVICE_PERMISSION_DENIED;
    }
    return ret;
}
int32_t UsbService::HasRight(std::string deviceName)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbRightManager HasRight");
    std::string bundleName;
    if (GetBundleName(bundleName)) {
        USB_HILOGI(MODULE_USB_SERVICE, "HasRight bundleName = %{public}s", bundleName.c_str());
        return usbRightManager->HasRight(deviceName, bundleName);
    }
    USB_HILOGE(MODULE_USB_SERVICE, "HasRight GetBundleName false");
    return UEC_SERVICE_INNER_ERR;
}

int32_t UsbService::RequestRight(std::string deviceName)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbRightManager RequestRight");
    std::string bundleName;
    if (GetBundleName(bundleName)) {
        USB_HILOGI(MODULE_USB_SERVICE, "RequestRight bundleName = %{public}s", bundleName.c_str());
        return usbRightManager->RequestRight(deviceName, bundleName);
    }
    USB_HILOGI(MODULE_USB_SERVICE, "RequestRight GetBundleName false");
    return UEC_SERVICE_INNER_ERR;
}

int32_t UsbService::RemoveRight(std::string deviceName)
{
    if (usbRightManager->RemoveDeviceRight(deviceName)) {
        return UEC_OK;
    }
    return UEC_SERVICE_INNER_ERR;
}

int32_t UsbService::GetDevices(std::vector<UsbDevice> &deviceList)
{
    std::map<std::string, UsbDevice *> devices;
    usbHostManger_->getDevices(devices);
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s list size %{public}d", __func__, devices.size());
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        deviceList.push_back(*it->second);
    }
    return UEC_OK;
}

int32_t UsbService::GetCurrentFunctions(int32_t &funcs)
{
    return UsbdClient::GetCurrentFunctions(funcs);
}

int32_t UsbService::SetCurrentFunctions(int32_t funcs)
{
    USB_HILOGI(MODULE_USB_SERVICE, "func = %{public}d", funcs);
    return UsbdClient::SetCurrentFunctions(funcs);
}

int32_t UsbService::UsbFunctionsFromString(std::string funcs)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling UsbFunctionsFromString");
    return UsbFunctionManager::FromStringFunctions(funcs);
}

std::string UsbService::UsbFunctionsToString(int32_t funcs)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling UsbFunctionsToString");
    return UsbFunctionManager::ToStringFunctions(funcs);
}

int32_t UsbService::GetPorts(std::vector<UsbPort> &ports)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbPortManager getPorts");
    return usbPortManager->GetPorts(ports);
}

int32_t UsbService::GetSupportedModes(int32_t portId, int32_t &supportedModes)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbPortManager getSupportedModes");
    return usbPortManager->GetSupportedModes(portId, supportedModes);
}

int32_t UsbService::SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole)
{
    USB_HILOGI(MODULE_USB_SERVICE, "calling usbd getPorts");
    return UsbdClient::SetPortRole(portId, powerRole, dataRole);
}

int32_t UsbService::ClaimInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::ClaimInterface(dev, interface);
}
int32_t UsbService::ReleaseInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::ReleaseInterface(dev, interface);
}
int32_t UsbService::BulkTransferRead(const UsbDev &devInfo,
                                     const UsbPipe &pipe,
                                     std::vector<uint8_t> &vdata,
                                     int32_t timeout)
{
    int32_t ret = UsbdClient::BulkTransferRead(devInfo, pipe, timeout, vdata);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d BulkTransferRead error ret:%{public}d", __func__,
                   __LINE__, ret);
    }
    return ret;
}
int32_t UsbService::BulkTransferWrite(const UsbDev &dev,
                                      const UsbPipe &pipe,
                                      const std::vector<uint8_t> &vdata,
                                      int32_t timeout)
{
    int32_t ret = UsbdClient::BulkTransferWrite(dev, pipe, timeout, vdata);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d BulkTransferWrite error ret:%{public}d", __func__,
                   __LINE__, ret);
    }
    return ret;
}
int32_t UsbService::ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &vdata)
{
    int32_t ret = UsbdClient::ControlTransfer(dev, ctrl, vdata);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d error ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}
int32_t UsbService::SetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t configIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::SetConfig(dev, configIndex);
}
int32_t UsbService::GetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t &configIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::GetConfig(dev, configIndex);
}
int32_t UsbService::SetInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t altIndex)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::SetInterface(dev, interfaceid, altIndex);
}
int32_t UsbService::GetRawDescriptor(uint8_t busNum, uint8_t devAddr, std::vector<uint8_t> &vdata)
{
    const UsbDev dev = {busNum, devAddr};
    int32_t ret = UsbdClient::GetRawDescriptor(dev, vdata);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d error ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}
int32_t UsbService::RequestQueue(const UsbDev &dev,
                                 const UsbPipe &pipe,
                                 const std::vector<uint8_t> &cData,
                                 const std::vector<uint8_t> &vData)
{
    int32_t ret = UsbdClient::RequestQueue(dev, pipe, cData, vData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d error ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}
int32_t UsbService::RequestWait(const UsbDev &dev,
                                int32_t timeout,
                                std::vector<uint8_t> &cData,
                                std::vector<uint8_t> &vData)
{
    int32_t ret = UsbdClient::RequestWait(dev, cData, vData, timeout);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:%{public}d error ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbService::RequestCancel(uint8_t busNum, uint8_t devAddr, uint8_t interfaceId, uint8_t endpointId)
{
    const UsbDev dev = {busNum, devAddr};
    const UsbPipe pipe = {interfaceId, endpointId};
    return UsbdClient::RequestCancel(dev, pipe);
}

int32_t UsbService::Close(uint8_t busNum, uint8_t devAddr)
{
    const UsbDev dev = {busNum, devAddr};
    return UsbdClient::CloseDevice(dev);
}

static std::string GetDevStringValFromIdx(uint8_t busNum, uint8_t devAddr, uint8_t idx)
{
    const UsbDev dev = {busNum, devAddr};
    std::vector<uint8_t> strV;
    uint32_t length = 0;
    std::string string = " ";

    USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d getString idx:%{public}d", __func__, __LINE__, idx);
    if (idx == 0) {
        return string;
    }

    int32_t ret = UsbdClient::GetStringDescriptor(dev, idx, strV);
    if (ret != UEC_OK) {
        USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d get string[%{public}d] failed ret:%{public}d", __func__,
                   __LINE__, idx, ret);
        return string;
    }
    length = strV.size();
    if ((length < DESCRIPTOR_VALUE_START_OFFSET) || (strV[1] != DESCRIPTOR_TYPE_STRING)) {
        USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d type or length error, len:%{public}d", __func__, __LINE__,
                   length);
        return string;
    }
    uint16_t *tbuf = new uint16_t[length + 1]();
    for (uint32_t i = 0; i < length - DESCRIPTOR_VALUE_START_OFFSET; ++i) {
        tbuf[i] = strV[i + DESCRIPTOR_VALUE_START_OFFSET];
    }
    std::wstring wstr((wchar_t *)(tbuf), (length - DESCRIPTOR_VALUE_START_OFFSET) / HALF);
    string = std::string(wstr.begin(), wstr.end());
    USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d getString idx:%{public}d String:%{public}s length:%{public}d",
               __func__, __LINE__, idx, string.c_str(), length);
    delete[] tbuf;
    return string;
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

    busNum = dev.GetBusNum();
    devAddr = dev.GetDevAddr();
    uint16_t bcdDevice = dev.GetbcdDevice();
    uint8_t *ptr = (uint8_t *)&bcdDevice;
    const std::vector<uint8_t> bcdData(ptr, ptr + DESCRIPTOR_VALUE_START_OFFSET);
    dev.SetVersion(BcdToString(bcdData));
    dev.SetManufacturerName(GetDevStringValFromIdx(busNum, devAddr, dev.GetiManufacturer()));
    dev.SetProductName(GetDevStringValFromIdx(busNum, devAddr, dev.GetiProduct()));
    dev.SetmSerial(GetDevStringValFromIdx(busNum, devAddr, dev.GetiSerialNumber()));
    USB_HILOGW(MODULE_USB_SERVICE,
               "%{public}s:%{public}d iSerial:%{public}d mSerial:%{public}s Manufactur:%{public}s product:%{public}s "
               "version:%{public}s",
               __func__, __LINE__, dev.GetiSerialNumber(), dev.GetmSerial().c_str(), dev.GetManufacturerName().c_str(),
               dev.GetProductName().c_str(), dev.GetVersion().c_str());

    std::vector<USBConfig> configs;
    configs = dev.GetConfigs();
    for (auto it = configs.begin(); it != configs.end(); ++it) {
        it->SetName(GetDevStringValFromIdx(busNum, devAddr, it->GetiConfiguration()));
        USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d Config:%{public}d %{public}s", __func__, __LINE__,
                   it->GetiConfiguration(), it->GetName().c_str());
        std::vector<UsbInterface> interfaces = it->GetInterfaces();
        for (auto itIF = interfaces.begin(); itIF != interfaces.end(); ++itIF) {
            itIF->SetName(GetDevStringValFromIdx(busNum, devAddr, itIF->GetiInterface()));
            USB_HILOGW(MODULE_USB_SERVICE, "%{public}s:%{public}d interface:%{public}d %{public}s", __func__, __LINE__,
                       itIF->GetiInterface(), itIF->GetName().c_str());
        }
        it->SetInterfaces(interfaces);
    }
    dev.SetConfigs(configs);

    return UEC_OK;
}

int32_t UsbService::GetDeviceInfoDescriptor(const UsbDev &uDev,
                                            std::vector<uint8_t> &decriptor,
                                            uint32_t &length,
                                            UsbDevice &dev)
{
    uint8_t *buffer = NULL;
    int32_t ret = UsbdClient::GetDeviceDescriptor(uDev, decriptor);
    if (ret != UEC_OK) {
        UsbdClient::CloseDevice(uDev);
        USB_HILOGE(MODULE_USB_SERVICE,
                   "%{public}s:%{public}d UsbdClient::GetDeviceDescriptor failed ret=%{public}d busNum:%{public}d "
                   "devAddr:%{public}d",
                   __func__, __LINE__, ret, uDev.busNum, uDev.devAddr);
        return ret;
    }
    buffer = decriptor.data();
    length = decriptor.size();
    if ((!buffer) || (length == 0)) {
        USB_HILOGE(MODULE_USB_SERVICE,
                   "%{public}s:%{public}d UsbdClient::GetDeviceDescriptor failed len=%{public}d busNum:%{public}d "
                   "devAddr:%{public}d",
                   __func__, __LINE__, length, uDev.busNum, uDev.devAddr);
        return UEC_SERVICE_INVALID_VALUE;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d GetDeviceDescriptor ret=%{public}d len:%{public}d", __func__,
               __LINE__, ret, length);
    dev.SetBusNum(uDev.busNum);
    dev.SetDevAddr(uDev.devAddr);
    dev.SetName(std::to_string(uDev.busNum) + "-" + std::to_string(uDev.devAddr));
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s GetDeviceDescriptor length=%{public}d", __func__, length);
    ret = UsbDescriptorParser::ParseDeviceDescriptor(buffer, length, dev);
    if (ret != UEC_OK) {
        UsbdClient::CloseDevice(uDev);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s ParseDeviceDescriptor failed ret=%{public}d", __func__, ret);
        return ret;
    }
    return ret;
}

int32_t UsbService::GetConfigDescriptor(const UsbDev &uDev,
                                        std::vector<uint8_t> &decriptor,
                                        std::vector<USBConfig> &configs,
                                        UsbDevice &dev)
{
    uint8_t *buffer = NULL;
    int32_t ret = UEC_OK;
    for (uint8_t i = 0; i < dev.GetDescConfigCount(); ++i) {
        uint32_t cursor = 0;
        decriptor.clear();
        uint32_t length = 0;
        ret = UsbdClient::GetConfigDescriptor(uDev, i, decriptor);
        if (ret != UEC_OK) {
            UsbdClient::CloseDevice(uDev);
            USB_HILOGI(MODULE_USB_SERVICE, "%{public}s UsbdClient::GetConfigDescriptor failed ret=%{public}d", __func__,
                       ret);
            return ret;
        }
        length = decriptor.size();
        buffer = decriptor.data();
        if (length == 0) {
            USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d GetConfigDescriptor[%{public}d] length=%{public}d",
                       __func__, __LINE__, i, length);
            continue;
        }
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s GetConfigDescriptor length=%{public}d", __func__, length);
        uint32_t configCursor = 0;
        USBConfig config;
        ret = UsbDescriptorParser::ParseConfigDescriptor(buffer + cursor, length - cursor, configCursor, config);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s ParseConfigDescriptor[%{public}d] ret:%{public}d", __func__, i, ret);
        cursor += configCursor;
        configs.push_back(config);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s Descriptor=%{public}s", __func__, config.ToString().c_str());
    }
    return ret;
}

int32_t UsbService::GetDeviceInfo(uint8_t busNum, uint8_t devAddr, UsbDevice &dev)
{
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d busNum:%{public}d devAddr:%{public}d", __func__, __LINE__,
               busNum, devAddr);
    const UsbDev uDev = {busNum, devAddr};
    std::vector<uint8_t> decriptor;
    uint32_t length = USB_MAX_DESCRIPTOR_SIZE;

    int32_t ret = UsbdClient::OpenDevice(uDev);
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d UsbdClient::OpenDevice ret=%{public}d", __func__, __LINE__,
               ret);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s UsbdClient::OpenDevice failed ret=%{public}d", __func__, ret);
        return ret;
    }

    ret = GetDeviceInfoDescriptor(uDev, decriptor, length, dev);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s UsbdClient::GetDeviceInfoDescriptor failed ret=%{public}d", __func__,
                   ret);
        return ret;
    }

    std::vector<USBConfig> configs;
    ret = GetConfigDescriptor(uDev, decriptor, configs, dev);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s UsbdClient::GetConfigDescriptor failed ret=%{public}d", __func__,
                   ret);
        return ret;
    }
    dev.SetConfigs(configs);
    ret = FillDevStrings(dev);
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d FillDevStrings ret=%{public}d", __func__, __LINE__, ret);

    UsbdClient::CloseDevice(uDev);
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s Descriptor=%{public}s", __func__, dev.ToString().c_str());

    return UEC_OK;
}

bool UsbService::AddDevice(uint8_t busNum, uint8_t devAddr)
{
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d busNum:%{public}d devAddr:%{public}d", __func__, __LINE__, busNum,
               devAddr);
    UsbDevice *devInfo = new UsbDevice();
    memset_s(devInfo, sizeof(UsbDevice), 0, sizeof(UsbDevice));
    int32_t ret = GetDeviceInfo(busNum, devAddr, *devInfo);
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:%{public}d GetDeviceInfo ret=%{public}d", __func__, __LINE__, ret);
    if (ret == UEC_OK) {
        usbHostManger_->AddDevice(devInfo);
        return true;
    }
    delete devInfo;
    return false;
}
bool UsbService::DelDevice(uint8_t busNum, uint8_t devAddr)
{
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d entry", __func__, __LINE__);
    int32_t ret = Close(busNum, devAddr);
    if (ret != UEC_OK) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d Close device failed width ret = %{public}d", __func__, __LINE__,
                   ret);
    }
    return usbHostManger_->DelDevice(busNum, devAddr);
}

void UsbService::UpdateUsbPort(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode)
{
    usbPortManager->UpdatePort(portId, powerRole, dataRole, mode);
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
} // namespace USB
} // namespace OHOS
