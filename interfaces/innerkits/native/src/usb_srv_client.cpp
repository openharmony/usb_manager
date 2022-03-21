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

#include "usb_srv_client.h"
#include <sstream>
#include "datetime_ex.h"
#include "if_system_ability_manager.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "string_ex.h"
#include "system_ability_definition.h"
#include "usb_common.h"
#include "usb_device.h"
#include "usb_errors.h"

namespace OHOS {
namespace USB {
const uint8_t CLAIM_FORCE_1 = 1;
UsbSrvClient::UsbSrvClient()
{
    Connect();
}
UsbSrvClient::~UsbSrvClient() {}

int32_t UsbSrvClient::Connect()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (proxy_ != nullptr) {
        return UEC_OK;
    }
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sm == nullptr) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s:fail to get Registry", __func__);
        return UEC_INTERFACE_GET_SYSTEM_ABILITY_MANAGER_FAILED;
    }
    sptr<IRemoteObject> remoteObject_ = sm->CheckSystemAbility(USB_SYSTEM_ABILITY_ID);
    if (remoteObject_ == nullptr) {
        USB_HILOGE(MODULE_USB_INNERKIT, "GetSystemAbility failed.");
        return UEC_INTERFACE_GET_USB_SERVICE_FAILED;
    }
    proxy_ = iface_cast<IUsbSrv>(remoteObject_);
    USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s :Connect UsbService ok.", __func__);
    return UEC_OK;
}

void UsbSrvClient::ResetProxy(const wptr<IRemoteObject> &remote)
{
    std::lock_guard<std::mutex> lock(mutex_);
    RETURN_IF(proxy_ == nullptr);
    auto serviceRemote = proxy_->AsObject();
    if ((serviceRemote != nullptr) && (serviceRemote == remote.promote())) {
        serviceRemote->RemoveDeathRecipient(deathRecipient_);
        proxy_ = nullptr;
    }
}

void UsbSrvClient::UsbSrvDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    if (remote == nullptr) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbSrvDeathRecipient::OnRemoteDied failed, remote is nullptr.");
        return;
    }
    UsbSrvClient::GetInstance().ResetProxy(remote);
    USB_HILOGI(MODULE_USB_INNERKIT, "UsbSrvDeathRecipient::Recv death notice.");
}

int32_t UsbSrvClient::OpenDevice(const UsbDevice &device, USBDevicePipe &pipe)
{
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling OpenDevice Start!");
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->OpenDevice(device.GetBusNum(), device.GetDevAddr());
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
        return ret;
    }

    pipe.SetBusNum(device.GetBusNum());
    pipe.SetDevAddr(device.GetDevAddr());
    return UEC_OK;
}

int32_t UsbSrvClient::HasRight(std::string deviceName)
{
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling HasRight Start!");
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->HasRight(deviceName);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, " Calling HasRight False!");
    }
    return ret;
}

int32_t UsbSrvClient::RequestRight(std::string deviceName)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->RequestRight(deviceName);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, " Calling RequestRight False!");
    }
    return ret;
}

int32_t UsbSrvClient::RemoveRight(std::string deviceName)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->RemoveRight(deviceName);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, " Calling RequestRight False!");
    }
    return ret;
}

int32_t UsbSrvClient::GetDevices(std::vector<UsbDevice> &deviceList)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->GetDevices(deviceList);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s list size = %{public}d!", __func__, deviceList.size());
    return ret;
}

int32_t UsbSrvClient::GetCurrentFunctions(int32_t &funcs)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->GetCurrentFunctions(funcs);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling GetCurrentFunctions Success!");
    return ret;
}

int32_t UsbSrvClient::SetCurrentFunctions(int32_t funcs)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, false);
    int32_t ret = proxy_->SetCurrentFunctions(funcs);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
        return ret;
    }
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling SetCurrentFunctions Success!");
    return ret;
}

int32_t UsbSrvClient::UsbFunctionsFromString(std::string_view funcs)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t result = proxy_->UsbFunctionsFromString(funcs);
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling UsbFunctionsFromString Success!");
    return result;
}

std::string UsbSrvClient::UsbFunctionsToString(int32_t funcs)
{
    std::string result;
    RETURN_IF_WITH_RET(Connect() != UEC_OK, result);
    result = proxy_->UsbFunctionsToString(funcs);
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling UsbFunctionsToString Success!");
    return result;
}

int32_t UsbSrvClient::GetPorts(std::vector<UsbPort> &usbports)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling GetPorts");
    int32_t ret = proxy_->GetPorts(usbports);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::GetSupportedModes(int32_t portId, int32_t &result)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling GetSupportedModes");
    int32_t ret = proxy_->GetSupportedModes(portId, result);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    USB_HILOGI(MODULE_USB_INNERKIT, " Calling SetPortRole");
    int32_t ret = proxy_->SetPortRole(portId, powerRole, dataRole);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::ClaimInterface(USBDevicePipe &pipe, const UsbInterface &interface, bool force)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->ClaimInterface(pipe.GetBusNum(), pipe.GetDevAddr(), interface.GetId(), force);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::ReleaseInterface(USBDevicePipe &pipe, const UsbInterface &interface)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->ReleaseInterface(pipe.GetBusNum(), pipe.GetDevAddr(), interface.GetId());
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::BulkTransfer(USBDevicePipe &pipe, const USBEndpoint &endpoint, std::vector<uint8_t> &bufferData,
    int32_t timeOut)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    int32_t ret = UEC_INTERFACE_INVALID_VALUE;
    const UsbDev tdev = {pipe.GetBusNum(), pipe.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    if (USB_ENDPOINT_DIR_IN == endpoint.GetDirection()) {
        ret = proxy_->BulkTransferRead(tdev, tpipe, bufferData, timeOut);
    } else if (USB_ENDPOINT_DIR_OUT == endpoint.GetDirection()) {
        ret = proxy_->BulkTransferWrite(tdev, tpipe, bufferData, timeOut);
    }
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::ControlTransfer(USBDevicePipe &pipe, const UsbCtrlTransfer &ctrl,
    std::vector<uint8_t> &bufferData)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev dev = {pipe.GetBusNum(), pipe.GetDevAddr()};
    int32_t ret = proxy_->ControlTransfer(dev, ctrl, bufferData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }

    return ret;
}

int32_t UsbSrvClient::SetConfiguration(USBDevicePipe &pipe, const USBConfig &config)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->SetActiveConfig(pipe.GetBusNum(), pipe.GetDevAddr(), config.GetId());
    return ret;
}

int32_t UsbSrvClient::SetInterface(USBDevicePipe &pipe, const UsbInterface &interface)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    return proxy_->SetInterface(pipe.GetBusNum(), pipe.GetDevAddr(), interface.GetId(),
                                interface.GetAlternateSetting());
}

int32_t UsbSrvClient::GetRawDescriptors(USBDevicePipe &pipe, std::vector<uint8_t> &bufferData)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->GetRawDescriptor(pipe.GetBusNum(), pipe.GetDevAddr(), bufferData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::GetFileDescriptor(USBDevicePipe &pipe, int32_t &fd)
{
    RETURN_IF_WITH_RET(Connect() != UEC_OK, UEC_INTERFACE_NO_INIT);
    int32_t ret = proxy_->GetFileDescriptor(pipe.GetBusNum(), pipe.GetDevAddr(), fd);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s failed ret = %{public}d!", __func__, ret);
    }
    return ret;
}

bool UsbSrvClient::Close(const USBDevicePipe &pipe)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, false);
    int32_t ret = proxy_->Close(pipe.GetBusNum(), pipe.GetDevAddr());
    return (ret == UEC_OK);
}

int32_t UsbSrvClient::PipeRequestWait(USBDevicePipe &pipe, int64_t timeOut, UsbRequest &req)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    std::vector<uint8_t> clientData;
    std::vector<uint8_t> bufferData;
    const UsbDev tdev = {pipe.GetBusNum(), pipe.GetDevAddr()};
    int32_t ret = proxy_->RequestWait(tdev, timeOut, clientData, bufferData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbSrvClient::%{public}s:%{public}d :failed width ret = %{public}d.", __func__,
                   __LINE__, ret);
        return ret;
    }

    req.SetPipe(pipe);
    req.SetClientData(clientData);
    req.SetReqData(bufferData);
    return ret;
}

int32_t UsbSrvClient::RequestInitialize(UsbRequest &request)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const USBDevicePipe &pipe = request.GetPipe();
    const USBEndpoint &endpoint = request.GetEndpoint();
    return proxy_->ClaimInterface(pipe.GetBusNum(), pipe.GetDevAddr(), endpoint.GetInterfaceId(), CLAIM_FORCE_1);
}

int32_t UsbSrvClient::RequestFree(UsbRequest &request)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const USBDevicePipe &pipe = request.GetPipe();
    const USBEndpoint &ep = request.GetEndpoint();
    return proxy_->RequestCancel(pipe.GetBusNum(), pipe.GetDevAddr(), ep.GetInterfaceId(), ep.GetAddress());
}

int32_t UsbSrvClient::RequestAbort(UsbRequest &request)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const USBDevicePipe &pipe = request.GetPipe();
    const USBEndpoint &ep = request.GetEndpoint();
    return proxy_->RequestCancel(pipe.GetBusNum(), pipe.GetDevAddr(), ep.GetInterfaceId(), ep.GetAddress());
}

int32_t UsbSrvClient::RequestQueue(UsbRequest &request)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const USBDevicePipe &pipe = request.GetPipe();
    const USBEndpoint &ep = request.GetEndpoint();
    const UsbDev tdev = {pipe.GetBusNum(), pipe.GetDevAddr()};
    const UsbPipe tpipe = {ep.GetInterfaceId(), ep.GetAddress()};
    return proxy_->RequestQueue(tdev, tpipe, request.GetClientData(), request.GetReqData());
}

int32_t UsbSrvClient::RegBulkCallback(USBDevicePipe &pip, const USBEndpoint &endpoint, const sptr<IRemoteObject> &cb)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev tdev = {pip.GetBusNum(), pip.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    int32_t ret = proxy_->RegBulkCallback(tdev, tpipe, cb);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::UnRegBulkCallback(USBDevicePipe &pip, const USBEndpoint &endpoint)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev tdev = {pip.GetBusNum(), pip.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    int32_t ret = proxy_->UnRegBulkCallback(tdev, tpipe);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::BulkRead(USBDevicePipe &pip, const USBEndpoint &endpoint, sptr<Ashmem> &ashmem)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev tdev = {pip.GetBusNum(), pip.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    int32_t ret = proxy_->BulkRead(tdev, tpipe, ashmem);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::BulkWrite(USBDevicePipe &pip, const USBEndpoint &endpoint, sptr<Ashmem> &ashmem)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev tdev = {pip.GetBusNum(), pip.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    int32_t ret = proxy_->BulkWrite(tdev, tpipe, ashmem);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}

int32_t UsbSrvClient::BulkCancel(USBDevicePipe &pip, const USBEndpoint &endpoint)
{
    RETURN_IF_WITH_RET(proxy_ == nullptr, UEC_INTERFACE_NO_INIT);
    const UsbDev tdev = {pip.GetBusNum(), pip.GetDevAddr()};
    const UsbPipe tpipe = {endpoint.GetInterfaceId(), endpoint.GetAddress()};
    int32_t ret = proxy_->BulkCancel(tdev, tpipe);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s : failed width ret = %{public}d !", __func__, ret);
    }
    return ret;
}
} // namespace USB
} // namespace OHOS
