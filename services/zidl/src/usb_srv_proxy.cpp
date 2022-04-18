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

#include <map>
#include <sstream>
#include "ipc_types.h"
#include "message_parcel.h"
#include "securec.h"
#include "usb_common.h"
#include "usb_errors.h"
#include "usb_request.h"
#include "usb_server_proxy.h"

namespace OHOS {
namespace USB {
int32_t UsbServerProxy::SetDeviceMessage(MessageParcel &data, uint8_t busNum, uint8_t devAddr)
{
    WRITE_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return UEC_OK;
}

int32_t UsbServerProxy::SetBufferMessage(MessageParcel &data, const std::vector<uint8_t> &bufferData)
{
    uint32_t length = bufferData.size();
    const uint8_t *ptr = bufferData.data();
    if (!ptr) {
        length = 0;
    }

    if (!data.WriteUint32(length)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed length:%{public}u", __func__, __LINE__, length);
        return UEC_SERVICE_WRITE_PARCEL_ERROR;
    }
    if ((ptr) && (length > 0) && !data.WriteBuffer((const void *)ptr, length)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed length:%{public}u", __func__, __LINE__, length);
        return UEC_SERVICE_WRITE_PARCEL_ERROR;
    } else {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d success length:%{public}u", __func__, __LINE__, length);
    }
    return UEC_OK;
}

int32_t UsbServerProxy::GetBufferMessage(MessageParcel &data, std::vector<uint8_t> &bufferData)
{
    uint32_t dataSize = 0;
    bufferData.clear();
    if (!data.ReadUint32(dataSize)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed", __func__, __LINE__);
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    if (dataSize == 0) {
        USB_HILOGI(MODULE_USBD, "%{public}s:%{public}d size:%{public}u", __func__, __LINE__, dataSize);
        return UEC_OK;
    }

    const uint8_t *readData = data.ReadUnpadBuffer(dataSize);
    if (readData == nullptr) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed size:%{public}u", __func__, __LINE__, dataSize);
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    std::vector<uint8_t> tdata(readData, readData + dataSize);
    bufferData.swap(tdata);
    return UEC_OK;
}

int32_t UsbServerProxy::GetDevices(std::vector<UsbDevice> &deviceList)
{
    int32_t ret;
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s remote is failed", __func__);
        return ERR_INVALID_VALUE;
    }
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_INVALID_VALUE;
    }

    ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_DEVICES), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s failed code: %{public}d", __func__, ret);
        return ret;
    }
    ret = GetDeviceListMessageParcel(reply, deviceList);
    return ret;
}

int32_t UsbServerProxy::GetDeviceListMessageParcel(MessageParcel &data, std::vector<UsbDevice> &deviceList)
{
    int32_t count;
    data.ReadInt32(count);

    for (int32_t i = 0; i < count; ++i) {
        UsbDevice devInfo;
        GetDeviceMessageParcel(data, devInfo);
        deviceList.push_back(devInfo);
    }
    return UEC_OK;
}

int32_t UsbServerProxy::GetDeviceMessageParcel(MessageParcel &data, UsbDevice &devInfo)
{
    int32_t tmp;
    uint8_t tui8;
    uint16_t tui16;
    std::string tstr;
    data.ReadInt32(tmp);
    devInfo.SetBusNum(tmp);
    data.ReadInt32(tmp);
    devInfo.SetDevAddr(tmp);

    data.ReadInt32(tmp);
    devInfo.SetVendorId(tmp);
    data.ReadInt32(tmp);
    devInfo.SetProductId(tmp);
    data.ReadInt32(tmp);
    devInfo.SetClass(tmp);
    data.ReadInt32(tmp);
    devInfo.SetSubclass(tmp);
    data.ReadInt32(tmp);
    devInfo.SetProtocol(tmp);
    data.ReadUint8(tui8);
    devInfo.SetiManufacturer(tui8);
    data.ReadUint8(tui8);
    devInfo.SetiProduct(tui8);
    data.ReadUint8(tui8);
    devInfo.SetiSerialNumber(tui8);
    data.ReadUint8(tui8);
    devInfo.SetbMaxPacketSize0(tui8);
    data.ReadUint16(tui16);
    devInfo.SetbcdUSB(tui16);
    data.ReadUint16(tui16);
    devInfo.SetbcdDevice(tui16);
    data.ReadString(tstr);
    devInfo.SetName(tstr);
    data.ReadString(tstr);
    devInfo.SetManufacturerName(tstr);
    data.ReadString(tstr);
    devInfo.SetProductName(tstr);
    data.ReadString(tstr);
    devInfo.SetVersion(tstr);
    data.ReadString(tstr);
    devInfo.SetmSerial(tstr);

    USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s:%{public}d devName:%{public}s Bus:%{public}d dev:%{public}d ", __func__,
               __LINE__, devInfo.GetName().c_str(), devInfo.GetBusNum(), devInfo.GetDevAddr());
    std::vector<USBConfig> configs;
    GetDeviceConfigsMessageParcel(data, configs);
    devInfo.SetConfigs(configs);
    USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s ToString : %{public}s", __func__, devInfo.ToString().c_str());
    return UEC_OK;
}

int32_t UsbServerProxy::GetDeviceConfigsMessageParcel(MessageParcel &data, std::vector<USBConfig> &configs)
{
    uint32_t configCount;
    uint8_t tui8;
    std::string tstr;
    data.ReadUint32(configCount);

    int32_t tmp;
    for (uint32_t i = 0; i < configCount; ++i) {
        USBConfig config;
        data.ReadInt32(tmp);
        config.SetId(tmp);
        data.ReadInt32(tmp);
        config.SetAttribute(tmp);
        data.ReadInt32(tmp);
        config.SetMaxPower(tmp);

        data.ReadUint8(tui8);
        config.SetiConfiguration(tui8);
        data.ReadString(tstr);
        config.SetName(tstr);

        std::vector<UsbInterface> interfaces;
        GetDeviceInterfacesMessageParcel(data, interfaces);
        config.SetInterfaces(interfaces);
        configs.push_back(config);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, config.ToString().c_str());
    }

    return UEC_OK;
}

int32_t UsbServerProxy::GetDeviceInterfacesMessageParcel(MessageParcel &data, std::vector<UsbInterface> &interfaces)
{
    int32_t tmp, interfaceCount;
    uint8_t tui8;
    std::string tstr;
    data.ReadInt32(tmp);
    interfaceCount = tmp;
    for (int32_t i = 0; i < interfaceCount; ++i) {
        UsbInterface interface;
        data.ReadInt32(tmp);
        interface.SetId(tmp);
        data.ReadInt32(tmp);
        interface.SetClass(tmp);
        data.ReadInt32(tmp);
        interface.SetSubClass(tmp);
        data.ReadInt32(tmp);
        interface.SetAlternateSetting(tmp);
        data.ReadInt32(tmp);
        interface.SetProtocol(tmp);

        data.ReadUint8(tui8);
        interface.SetiInterface(tui8);
        data.ReadString(tstr);
        interface.SetName(tstr);

        std::vector<USBEndpoint> eps;
        GetDeviceEndpointsMessageParcel(data, eps);
        for (size_t j = 0; j < eps.size(); ++j) {
            eps[j].SetInterfaceId(interface.GetId());
        }
        interface.SetEndpoints(eps);
        interfaces.push_back(interface);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, interface.ToString().c_str());
    }
    return UEC_OK;
}

int32_t UsbServerProxy::GetDeviceEndpointsMessageParcel(MessageParcel &data, std::vector<USBEndpoint> &eps)
{
    int32_t tmp, epCount;
    data.ReadInt32(tmp);
    epCount = tmp;
    for (int32_t i = 0; i < epCount; ++i) {
        USBEndpoint ep;
        data.ReadInt32(tmp);
        ep.SetAddr(tmp);
        data.ReadInt32(tmp);
        ep.SetAttr(tmp);
        data.ReadInt32(tmp);
        ep.SetInterval(tmp);
        data.ReadInt32(tmp);
        ep.SetMaxPacketSize(tmp);
        eps.push_back(ep);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, ep.ToString().c_str());
    }
    return UEC_OK;
}

int32_t UsbServerProxy::OpenDevice(uint8_t busNum, uint8_t devAddr)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }

    SetDeviceMessage(data, busNum, devAddr);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_OPEN_DEVICE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %{public}d",
                   __func__, ret);
        return ret;
    }
    return ret;
}

bool UsbServerProxy::HasRight(std::string deviceName)
{
    MessageParcel data;
    MessageOption option;
    MessageParcel reply;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, false);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return false;
    }

    WRITE_PARCEL_WITH_RET(data, String, deviceName, false);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_HAS_RIGHT), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %{public}d",
                   __func__, ret);
        return false;
    }

    bool result = false;
    READ_PARCEL_WITH_RET(reply, Bool, result, false);

    return result;
}

int32_t UsbServerProxy::RequestRight(std::string deviceName)
{
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, String, deviceName, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REQUEST_RIGHT), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %{public}d",
                   __func__, ret);
    }
    return ret;
}

int32_t UsbServerProxy::RemoveRight(std::string deviceName)
{
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, String, deviceName, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REMOVE_RIGHT), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
    }
    return ret;
}

int32_t UsbServerProxy::GetCurrentFunctions(int32_t &funcs)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);

    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }

    int32_t ret =
        remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_CURRENT_FUNCTIONS), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    READ_PARCEL_WITH_RET(reply, Int32, funcs, UEC_INTERFACE_READ_PARCEL_ERROR);
    return ret;
}

int32_t UsbServerProxy::SetCurrentFunctions(int32_t funcs)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);

    MessageOption option;
    MessageParcel data;
    MessageParcel reply;

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, Int32, funcs, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret =
        remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_SET_CURRENT_FUNCTIONS), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
    }
    return ret;
}

int32_t UsbServerProxy::UsbFunctionsFromString(std::string_view funcs)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, String, std::string { funcs }, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret =
        remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_USB_FUNCTIONS_FROM_STRING), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return UEC_INTERFACE_INVALID_VALUE;
    }
    int32_t result = 0;
    READ_PARCEL_WITH_RET(reply, Int32, result, INVALID_USB_INT_VALUE);
    return result;
}

std::string UsbServerProxy::UsbFunctionsToString(int32_t funcs)
{
    sptr<IRemoteObject> remote = Remote();

    MessageParcel data;
    MessageOption option;
    MessageParcel reply;

    RETURN_IF_WITH_RET(remote == nullptr, INVALID_STRING_VALUE);

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return INVALID_STRING_VALUE;
    }
    WRITE_PARCEL_WITH_RET(data, Int32, funcs, INVALID_STRING_VALUE);
    int32_t ret =
        remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_USB_FUNCTIONS_TO_STRING), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return INVALID_STRING_VALUE;
    }
    std::string result;
    READ_PARCEL_WITH_RET(reply, String, result, INVALID_STRING_VALUE);
    return result;
}

int32_t UsbServerProxy::GetPorts(std::vector<UsbPort> &ports)
{
    MessageOption option;
    sptr<IRemoteObject> remote = Remote();

    MessageParcel data;
    MessageParcel reply;
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_PORTS), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    int32_t size;
    READ_PARCEL_WITH_RET(reply, Int32, size, UEC_INTERFACE_READ_PARCEL_ERROR);
    USB_HILOGI(MODULE_USB_INNERKIT, "GetPorts size %{public}d", size);
    for (int32_t i = 0; i < size; ++i) {
        USB_HILOGI(MODULE_USB_INNERKIT, "ParseUsbPort : %{public}d", i);
        ret = ParseUsbPort(reply, ports);
        if (ret) {
            return ret;
        }
    }
    return ret;
}

int32_t UsbServerProxy::ParseUsbPort(MessageParcel &reply, std::vector<UsbPort> &ports)
{
    UsbPort port;
    UsbPortStatus status;
    READ_PARCEL_WITH_RET(reply, Int32, port.id, UEC_INTERFACE_READ_PARCEL_ERROR);
    USB_HILOGI(MODULE_USB_INNERKIT, "UsbServerProxy::port->id %{public}d", port.id);
    port.supportedModes = reply.ReadInt32();
    status.currentMode = reply.ReadInt32();
    status.currentPowerRole = reply.ReadInt32();
    status.currentDataRole = reply.ReadInt32();
    port.usbPortStatus = status;
    USB_HILOGI(MODULE_USB_INNERKIT, "UsbServerProxy::port.usbPortStatus.currentMode %{public}d",
               port.usbPortStatus.currentMode);
    ports.push_back(port);
    return UEC_OK;
}

int32_t UsbServerProxy::GetSupportedModes(int32_t portId, int32_t &supportedModes)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, Int32, portId, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_SUPPORTED_MODES), data, reply, option);
    if (ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    READ_PARCEL_WITH_RET(reply, Int32, supportedModes, UEC_INTERFACE_READ_PARCEL_ERROR);
    return ret;
}

int32_t UsbServerProxy::SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_INTERFACE_INVALID_VALUE);
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_INTERFACE_WRITE_PARCEL_ERROR;
    }
    WRITE_PARCEL_WITH_RET(data, Int32, portId, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, powerRole, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, dataRole, UEC_INTERFACE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_SET_PORT_ROLE), data, reply, option);
    if (ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::ClaimInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface, uint8_t force)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, force, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_CLAIM_INTERFACE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %{public}d",
                   __func__, ret);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::ReleaseInterface(uint8_t busNum, uint8_t devAddr, uint8_t interface)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_RELEASE_INTERFACE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
    }
    return ret;
}
int32_t UsbServerProxy::BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &bufferData,
    int32_t timeOut)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, timeOut, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_BULK_TRANSFER_READ), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s SendRequest is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    ret = GetBufferMessage(reply, bufferData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbServerProxy::%{public}s get buffer is failed, error code: %d", __func__,
                   ret);
        return ret;
    }
    USB_HILOGI(MODULE_USBD, "%{public}s Set buffer message. length = %{public}zu", __func__, bufferData.size());
    return ret;
}
int32_t UsbServerProxy::BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe,
    const std::vector<uint8_t> &bufferData, int32_t timeOut)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, timeOut, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = SetBufferMessage(data, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d SetBufferMessage ret:%{public}d", __func__,
                   __LINE__, ret);
        return ret;
    }
    ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_BULK_TRANSFER_WRITE), data, reply, option);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d SendRequest ret:%{public}d", __func__,
                   __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerProxy::ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
    std::vector<uint8_t> &bufferData)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return UEC_SERVICE_INNER_ERR;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Int32, ctrl.requestType, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, ctrl.requestCmd, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, ctrl.value, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, ctrl.index, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, ctrl.timeout, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = SetBufferMessage(data, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d write failed! len:%{public}d", __func__,
                   __LINE__, ret);
        return ret;
    }
    bool isWrite = ((ctrl.requestType & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_DIR_OUT);
    ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_CONTROL_TRANSFER), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d USB_FUN_CONTROL_TRANSFER ret:%{public}d",
                   __func__, __LINE__, ret);
        return ret;
    }
    if (!isWrite) {
        ret = GetBufferMessage(reply, bufferData);
        if (UEC_OK != ret) {
            USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d Get buffer message error. ret = %{public}d", __func__,
                       __LINE__, ret);
            return ret;
        }
        USB_HILOGI(MODULE_USBD, "%{public}s Get buffer message. length = %{public}zu", __func__, bufferData.size());
    }
    return UEC_OK;
}
int32_t UsbServerProxy::SetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t configIndex)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, configIndex, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_SET_ACTIVE_CONFIG), data, reply, option);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d USB_FUN_SET_ACTIVE_CONFIG ret:%{public}d",
                   __func__, __LINE__, ret);
    }
    return ret;
}
int32_t UsbServerProxy::GetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t &configIndex)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_ACTIVE_CONFIG), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d USB_FUN_GET_ACTIVE_CONFIG ret:%{public}d",
                   __func__, __LINE__, ret);
        return ret;
    }
    READ_PARCEL_WITH_RET(reply, Uint8, configIndex, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return ret;
}
int32_t UsbServerProxy::SetInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t altIndex)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, interfaceid, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, altIndex, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_SET_INTERFACE), data, reply, option);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d USB_FUN_SET_INTERFACE ret:%{public}d",
                   __func__, __LINE__, ret);
    }
    return ret;
}
int32_t UsbServerProxy::GetRawDescriptor(uint8_t busNum, uint8_t devAddr, std::vector<uint8_t> &bufferData)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_DESCRIPTOR), data, reply, option);
    if (ret == UEC_OK) {
        ret = GetBufferMessage(reply, bufferData);
        if (UEC_OK != ret) {
            USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d get failed ret:%{public}d", __func__,
                       __LINE__, ret);
        }
    }
    return ret;
}

int32_t UsbServerProxy::GetFileDescriptor(uint8_t busNum, uint8_t devAddr, int32_t &fd)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, busNum, devAddr);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_GET_FILEDESCRIPTOR), data, reply, option);
    if (ret == UEC_OK) {
        READ_PARCEL_WITH_RET(reply, Int32, fd, UEC_INTERFACE_READ_PARCEL_ERROR);
    }
    return ret;
}

int32_t UsbServerProxy::RequestQueue(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &clientData,
                                     const std::vector<uint8_t> &bufferData)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s get descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);

    int32_t ret = UsbServerProxy::SetBufferMessage(data, clientData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d setBuffer failed ret:%{public}d", __func__,
                   __LINE__, ret);
        return ERR_INVALID_VALUE;
    }

    ret = UsbServerProxy::SetBufferMessage(data, bufferData);
    if (UEC_OK != ret) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d setBuffer failed ret:%{public}d", __func__,
                   __LINE__, ret);
        return ERR_INVALID_VALUE;
    }

    ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REQUEST_QUEUE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d queue failed!", __func__, __LINE__);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::RequestWait(const UsbDev &dev, int32_t timeOut, std::vector<uint8_t> &clientData,
    std::vector<uint8_t> &bufferData)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s get descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }

    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Int32, timeOut, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REQUEST_WAIT), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d queue failed! ret:%{public}d", __func__,
                   __LINE__, ret);
        return ret;
    }

    ret = UsbServerProxy::GetBufferMessage(reply, clientData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d get clientData failed! ret:%{public}d",
                   __func__, __LINE__, ret);
        return ret;
    }

    ret = UsbServerProxy::GetBufferMessage(reply, bufferData);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s:%{public}d get buffer failed! ret:%{public}d", __func__,
                   __LINE__, ret);
        return ret;
    }

    return ret;
}

int32_t UsbServerProxy::RequestCancel(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t endpointId)
{
    int32_t ret;
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s get descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }

    SetDeviceMessage(data, busNum, devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, interfaceid, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REQUEST_CANCEL), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s request cancel failed!", __func__);
    }

    return ret;
}

int32_t UsbServerProxy::Close(uint8_t busNum, uint8_t devAddr)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s get descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }

    SetDeviceMessage(data, busNum, devAddr);
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_CLOSE_DEVICE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s queue failed!", __func__);
        return ret;
    }

    return UEC_OK;
}

int32_t UsbServerProxy::RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IRemoteObject> &cb)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, RemoteObject, cb, UEC_SERVICE_WRITE_PARCEL_ERROR);
    MessageOption option;
    MessageParcel reply;
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_REG_BULK_CALLBACK), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s SendRequest failed!", __func__);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    MessageOption option;
    MessageParcel reply;
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_UNREG_BULK_CALLBACK), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s SendRequest failed!", __func__);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::BulkRead(const UsbDev &dev, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Ashmem, ashmem, UEC_SERVICE_WRITE_PARCEL_ERROR);
    MessageOption option;
    MessageParcel reply;
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_BULK_AYSNC_READ), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s SendRequest failed!", __func__);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::BulkWrite(const UsbDev &dev, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Ashmem, ashmem, UEC_SERVICE_WRITE_PARCEL_ERROR);
    MessageOption option;
    MessageParcel reply;
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_BULK_AYSNC_WRITE), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s SendRequest failed!", __func__);
        return ret;
    }
    return ret;
}

int32_t UsbServerProxy::BulkCancel(const UsbDev &dev, const UsbPipe &pipe)
{
    sptr<IRemoteObject> remote = Remote();
    RETURN_IF_WITH_RET(remote == nullptr, UEC_SERVICE_INNER_ERR);
    MessageParcel data;
    if (!data.WriteInterfaceToken(UsbServerProxy::GetDescriptor())) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s write descriptor failed!", __func__);
        return ERR_ENOUGH_DATA;
    }
    SetDeviceMessage(data, dev.busNum, dev.devAddr);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, pipe.endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    MessageOption option;
    MessageParcel reply;
    int32_t ret = remote->SendRequest(static_cast<int32_t>(IUsbSrv::USB_FUN_BULK_AYSNC_CANCEL), data, reply, option);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_INNERKIT, "UsbServerProxy::%{public}s SendRequest failed!", __func__);
        return ret;
    }
    return ret;
}
} // namespace USB
} // namespace OHOS
