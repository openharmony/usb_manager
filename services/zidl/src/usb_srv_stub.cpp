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

#include <map>
#include <sstream>
#include <vector>
#include "message_parcel.h"
#include "securec.h"
#include "usb_common.h"
#include "usb_errors.h"
#include "usb_server_proxy.h"
#include "usb_server_stub.h"

namespace OHOS {
namespace USB {
int32_t UsbServerStub::GetDeviceMessage(MessageParcel &data, uint8_t &busNum, uint8_t &devAddr)
{
    if (!data.ReadUint8(busNum)) {
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    if (!data.ReadUint8(devAddr)) {
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    return UEC_OK;
}

int32_t UsbServerStub::SetBufferMessage(MessageParcel &data, const std::vector<uint8_t> &vdata)
{
    uint32_t length = vdata.size();
    const uint8_t *ptr = vdata.data();
    if (!ptr) {
        length = 0;
    }

    if (!data.WriteUint32(length)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed length:%{public}d", __func__, __LINE__, length);
        return UEC_SERVICE_WRITE_PARCEL_ERROR;
    }
    if ((ptr) && (length > 0) && !data.WriteBuffer((const void *)ptr, length)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed length:%{public}d", __func__, __LINE__, length);
        return UEC_SERVICE_WRITE_PARCEL_ERROR;
    }
    return UEC_OK;
}

int32_t UsbServerStub::GetBufferMessage(MessageParcel &data, std::vector<uint8_t> &vdata)
{
    uint32_t dataSize = 0;
    vdata.clear();
    if (!data.ReadUint32(dataSize)) {
        USB_HILOGE(MODULE_USBD, "%{public}s:%{public}d failed", __func__, __LINE__);
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    if (dataSize == 0) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d size:%{public}d", __func__, __LINE__, dataSize);
        return UEC_OK;
    }

    const uint8_t *readData = data.ReadUnpadBuffer(dataSize);
    if (readData == nullptr) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d failed size:%{public}d", __func__, __LINE__, dataSize);
        return UEC_SERVICE_READ_PARCEL_ERROR;
    }
    std::vector<uint8_t> tdata(readData, readData + dataSize);
    vdata.swap(tdata);
    return UEC_OK;
}

bool UsbServerStub::StubDevice(uint32_t code,
                               int32_t &result,
                               MessageParcel &data,
                               MessageParcel &reply,
                               MessageOption &option)
{
    switch (code) {
        case static_cast<int>(IUsbSrv::USB_FUN_OPEN_DEVICE):
            result = DoOpenDevice(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_HAS_RIGHT):
            result = DoHasRight(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_REQUEST_RIGHT):
            result = DoRequestRight(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_REMOVE_RIGHT):
            result = DoRemoveRight(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_GET_PORTS):
            result = DoGetPorts(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_GET_SUPPORTED_MODES):
            result = DoGetSupportedModes(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_SET_PORT_ROLE):
            result = DoSetPortRole(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_CLAIM_INTERFACE):
            result = DoClaimInterface(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_RELEASE_INTERFACE):
            result = DoReleaseInterface(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_BULK_TRANSFER_READ):
            result = DoBulkTransferRead(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_BULK_TRANSFER_WRITE):
            result = DoBulkTransferWrite(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_CONTROL_TRANSFER):
            result = DoControlTransfer(data, reply, option);
            return true;
        default:;
    }
    return false;
}

bool UsbServerStub::StubHost(uint32_t code,
                             int32_t &result,
                             MessageParcel &data,
                             MessageParcel &reply,
                             MessageOption &option)
{
    switch (code) {
        case static_cast<int>(IUsbSrv::USB_FUN_GET_DEVICES):
            result = DoGetDevices(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_GET_CURRENT_FUNCTIONS):
            result = DoGetCurrentFunctions(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_SET_CURRENT_FUNCTIONS):
            result = DoSetCurrentFunctions(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_USB_FUNCTIONS_FROM_STRING):
            result = DoUsbFunctionsFromString(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_USB_FUNCTIONS_TO_STRING):
            result = DoUsbFunctionsToString(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_CLOSE_DEVICE):
            result = DoClose(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_REQUEST_QUEUE):
            result = DoRequestQueue(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_REQUEST_WAIT):
            result = DoRequestWait(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_SET_INTERFACE):
            result = DoSetInterface(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_SET_ACTIVE_CONFIG):
            result = DoSetActiveConfig(data, reply, option);
            return true;
        case static_cast<int>(IUsbSrv::USB_FUN_REQUEST_CANCEL):
            result = DoRequestCancel(data, reply, option);
            return true;
        default:;
    }
    return false;
}

int UsbServerStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    USB_HILOGD(MODULE_USB_SERVICE, "UsbServerStub::OnRemoteRequest, cmd = %{public}d, flags = %{public}d", code,
               option.GetFlags());
    std::u16string descriptor = UsbServerStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        USB_HILOGE(MODULE_SERVICE, "UsbServerStub::OnRemoteRequest failed, descriptor is not matched!");
        return UEC_SERVICE_INNER_ERR;
    }

    int32_t ret = 0;
    if (StubHost(code, ret, data, reply, option)) {
        return ret;
    } else if (StubDevice(code, ret, data, reply, option)) {
        return ret;
    } else {
        return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }

    return UEC_OK;
}

int32_t UsbServerStub::DoGetCurrentFunctions(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t funcs;
    int32_t ret = GetCurrentFunctions(funcs);
    if (ret != UEC_OK) {
        return ret;
    }
    WRITE_PARCEL_WITH_RET(reply, Int32, funcs, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return UEC_OK;
}

int32_t UsbServerStub::DoSetCurrentFunctions(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t funcs;
    READ_PARCEL_WITH_RET(data, Int32, funcs, UEC_SERVICE_READ_PARCEL_ERROR);
    return SetCurrentFunctions(funcs);
}

int32_t UsbServerStub::DoUsbFunctionsFromString(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::string funcs;
    data.ReadString(funcs);
    int32_t ret = UsbFunctionsFromString(funcs);
    WRITE_PARCEL_WITH_RET(reply, Int32, ret, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return UEC_OK;
}

int32_t UsbServerStub::DoUsbFunctionsToString(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t funcs;
    data.ReadInt32(funcs);
    std::string ret = UsbFunctionsToString(funcs);
    WRITE_PARCEL_WITH_RET(reply, String, ret, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return UEC_OK;
}

int32_t UsbServerStub::DoOpenDevice(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_READ_PARCEL_ERROR);
    int32_t ret = OpenDevice(busNum, devAddr);
    if (ret != UEC_OK) {
        return ret;
    }

    return UEC_OK;
}

int32_t UsbServerStub::DoHasRight(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::string deviceName = "";
    READ_PARCEL_WITH_RET(data, String, deviceName, UEC_SERVICE_READ_PARCEL_ERROR);
    return HasRight(deviceName);
}

int32_t UsbServerStub::DoRequestRight(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::string deviceName = "";
    READ_PARCEL_WITH_RET(data, String, deviceName, UEC_SERVICE_READ_PARCEL_ERROR);
    return RequestRight(deviceName);
}

int32_t UsbServerStub::DoRemoveRight(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::string deviceName = "";
    READ_PARCEL_WITH_RET(data, String, deviceName, UEC_SERVICE_READ_PARCEL_ERROR);
    return RemoveRight(deviceName);
}

int32_t UsbServerStub::DoGetPorts(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::vector<UsbPort> ports;
    int ret = GetPorts(ports);
    USB_HILOGI(MODULE_SERVICE, "UsbServerStub::GetPorts ret %{public}d ", ret);
    if (ret != UEC_OK) {
        return ret;
    }
    unsigned int size = ports.size();
    USB_HILOGI(MODULE_SERVICE, "UsbServerStub::GetPorts size %{public}d ", size);
    WRITE_PARCEL_WITH_RET(reply, Int32, size, UEC_SERVICE_WRITE_PARCEL_ERROR);
    for (unsigned int i = 0; i < size; ++i) {
        ret = WriteUsbPort(reply, ports[i]);
        if (ret) {
            return ret;
        }
    }
    return ret;
}

int32_t UsbServerStub::WriteUsbPort(MessageParcel &reply, const UsbPort &port)
{
    WRITE_PARCEL_WITH_RET(reply, Int32, port.id, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(reply, Int32, port.supportedModes, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(reply, Int32, port.usbPortStatus.currentMode, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(reply, Int32, port.usbPortStatus.currentPowerRole, UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(reply, Int32, port.usbPortStatus.currentDataRole, UEC_SERVICE_WRITE_PARCEL_ERROR);
    USB_HILOGI(MODULE_SERVICE, "UsbServerStub::GetPorts supportedModes %{public}d ", port.supportedModes);
    return UEC_OK;
}

int32_t UsbServerStub::DoGetSupportedModes(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t supportedModes = 0;
    int32_t portId = 0;
    READ_PARCEL_WITH_RET(data, Int32, portId, UEC_SERVICE_READ_PARCEL_ERROR);
    int32_t ret = GetSupportedModes(portId, supportedModes);
    if (ret != UEC_OK) {
        return ret;
    }
    WRITE_PARCEL_WITH_RET(reply, Int32, supportedModes, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return ret;
}

int32_t UsbServerStub::DoSetPortRole(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t portId = 0;
    int32_t powerRole = 0;
    int32_t dataRole = 0;
    READ_PARCEL_WITH_RET(data, Int32, portId, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, powerRole, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, dataRole, UEC_SERVICE_READ_PARCEL_ERROR);
    return SetPortRole(portId, powerRole, dataRole);
}

int32_t UsbServerStub::DoClaimInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interface = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_READ_PARCEL_ERROR);
    return ClaimInterface(busNum, devAddr, interface);
}
int32_t UsbServerStub::DoReleaseInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interface = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_READ_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_READ_PARCEL_ERROR);
    return ReleaseInterface(busNum, devAddr, interface);
}

int32_t UsbServerStub::DoBulkTransferRead(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interface = 0;
    uint8_t endpoint = 0;
    int32_t timeout = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, endpoint, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, timeout, UEC_SERVICE_WRITE_PARCEL_ERROR);
    std::vector<uint8_t> vdata;
    const UsbDev tdev = {busNum, devAddr};
    const UsbPipe tpipe = {interface, endpoint};
    int32_t ret = BulkTransferRead(tdev, tpipe, vdata, timeout);
    if (ret != UEC_OK) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    ret = SetBufferMessage(reply, vdata);
    if (ret != UEC_OK) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::DoBulkTransferWrite(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interface = 0;
    uint8_t endpoint = 0;
    int32_t timeout = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interface, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, endpoint, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, timeout, UEC_SERVICE_WRITE_PARCEL_ERROR);
    std::vector<uint8_t> vdata;
    const UsbDev tdev = {busNum, devAddr};
    const UsbPipe tpipe = {interface, endpoint};
    int32_t ret = GetBufferMessage(data, vdata);
    if (UEC_OK != ret) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    ret = BulkTransferWrite(tdev, tpipe, vdata, timeout);
    if (UEC_OK != ret) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d BulkTransferWrite error ret:%{public}d", __func__, __LINE__,
                   ret);
    }
    return ret;
}

int32_t UsbServerStub::DoControlTransfer(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    int32_t requestType;
    int32_t request;
    int32_t value;
    int32_t index;
    int32_t timeout;

    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, requestType, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, request, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, value, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, index, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, timeout, UEC_SERVICE_WRITE_PARCEL_ERROR);
    std::vector<uint8_t> vdata;
    int32_t ret = GetBufferMessage(data, vdata);
    if (UEC_OK != ret) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d get error ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    // 写数据
    bool bWrite = ((requestType & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_DIR_OUT);
    const UsbDev tdev = {busNum, devAddr};
    const UsbCtrlTransfer tctrl = {requestType, request, value, index, timeout};
    ret = ControlTransfer(tdev, tctrl, vdata);
    if (ret != UEC_OK) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ControlTransfer error ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    // 读数据
    if (!bWrite) {
        ret = SetBufferMessage(reply, vdata);
        if (UEC_OK != ret) {
            USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d Set buffer message error length = %{public}d", __func__,
                       __LINE__, ret);
        }
    }

    return UEC_OK;
}

int32_t UsbServerStub::DoSetActiveConfig(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t config = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, config, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return SetActiveConfig(busNum, devAddr, config);
}

int32_t UsbServerStub::DoGetActiveConfig(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t config = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = GetActiveConfig(busNum, devAddr, config);
    if (ret == UEC_OK) {
        reply.WriteUint8(config);
    }
    return ret;
}

int32_t UsbServerStub::DoSetInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interfaceId = 0;
    uint8_t altIndex = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, altIndex, UEC_SERVICE_WRITE_PARCEL_ERROR);
    return SetInterface(busNum, devAddr, interfaceId, altIndex);
}

int32_t UsbServerStub::DoGetRawDescriptor(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    std::vector<uint8_t> vdata;
    int32_t ret = GetBufferMessage(data, vdata);
    if (UEC_OK != ret) {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    ret = GetRawDescriptor(busNum, devAddr, vdata);
    if (UEC_OK == ret) {
        ret = SetBufferMessage(reply, vdata);
        if (UEC_OK != ret) {
            USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
        }
    } else {
        USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::DoRequestQueue(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t ifId = 0;
    uint8_t endpoint = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, ifId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, endpoint, UEC_SERVICE_WRITE_PARCEL_ERROR);
    std::vector<uint8_t> cData;
    std::vector<uint8_t> vData;

    int32_t ret = UsbServerStub::GetBufferMessage(data, cData);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed  ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    ret = UsbServerStub::GetBufferMessage(data, vData);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed  ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    const UsbDev tdev = {busNum, devAddr};
    const UsbPipe tpipe = {ifId, endpoint};
    ret = RequestQueue(tdev, tpipe, cData, vData);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed  ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::DoRequestWait(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    int32_t timeout = 0;
    std::vector<uint8_t> cData;
    std::vector<uint8_t> vData;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Int32, timeout, UEC_SERVICE_WRITE_PARCEL_ERROR);

    const UsbDev tdev = {busNum, devAddr};
    int32_t ret = RequestWait(tdev, timeout, cData, vData);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }

    ret = SetBufferMessage(reply, cData);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }

    ret = SetBufferMessage(reply, vData);
    if (ret != UEC_OK) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
        return ret;
    }
    return ret;
}

int32_t UsbServerStub::DoRequestCancel(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    uint8_t interfaceId = 0;
    uint8_t endpointId = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, interfaceId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, endpointId, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = RequestCancel(busNum, devAddr, interfaceId, endpointId);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::DoClose(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    uint8_t busNum = 0;
    uint8_t devAddr = 0;
    READ_PARCEL_WITH_RET(data, Uint8, busNum, UEC_SERVICE_WRITE_PARCEL_ERROR);
    READ_PARCEL_WITH_RET(data, Uint8, devAddr, UEC_SERVICE_WRITE_PARCEL_ERROR);
    int32_t ret = Close(busNum, devAddr);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::DoGetDevices(MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    std::vector<UsbDevice> deviceList;
    int32_t ret = GetDevices(deviceList);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_SERVICE, "%{public}s: failed ret = %{public}d", __func__, ret);
        return ret;
    }
    USB_HILOGE(MODULE_SERVICE, "%{public}s: list size = %{public}d", __func__, deviceList.size());
    ret = SetDeviceListMessageParcel(deviceList, reply);
    if (UEC_OK != ret) {
        USB_HILOGI(MODULE_USB_INNERKIT, "%{public}s:%{public}d failed ret:%{public}d", __func__, __LINE__, ret);
    }
    return ret;
}

int32_t UsbServerStub::SetDeviceListMessageParcel(std::vector<UsbDevice> &deviceList, MessageParcel &data)
{
    int32_t deviceCount = (int32_t)deviceList.size();
    data.WriteInt32(deviceCount);
    for (int32_t i = 0; i < deviceCount; ++i) {
        UsbDevice &devInfo = deviceList[i];
        int32_t ret = SetDeviceMessageParcel(devInfo, data);
        if (ret) {
            return ret;
        }
    }
    return UEC_OK;
}

int32_t UsbServerStub::SetDeviceMessageParcel(UsbDevice &devInfo, MessageParcel &data)
{
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetBusNum(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetDevAddr(), UEC_SERVICE_WRITE_PARCEL_ERROR);

    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetVendorId(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetProductId(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetClass(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetSubclass(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetProtocol(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, devInfo.GetiManufacturer(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, devInfo.GetiProduct(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, devInfo.GetiSerialNumber(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint8, devInfo.GetbMaxPacketSize0(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint16, devInfo.GetbcdUSB(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, Uint16, devInfo.GetbcdDevice(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, String, devInfo.GetName(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, String, devInfo.GetManufacturerName(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, String, devInfo.GetProductName(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, String, devInfo.GetVersion(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    WRITE_PARCEL_WITH_RET(data, String, devInfo.GetmSerial(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s:%{public}d devName:%{public}s Bus:%{public}d dev:%{public}d ", __func__,
               __LINE__, devInfo.GetName().c_str(), devInfo.GetBusNum(), devInfo.GetDevAddr());
    USB_HILOGE(MODULE_USB_INNERKIT, "%{public}s:%{public}d devInfo:%{public}s", __func__, __LINE__,
               devInfo.ToString().c_str());
    WRITE_PARCEL_WITH_RET(data, Int32, devInfo.GetConfigCount(), UEC_SERVICE_WRITE_PARCEL_ERROR);
    return SetDeviceConfigsMessageParcel(devInfo.GetConfigs(), data);
}

int32_t UsbServerStub::SetDeviceConfigsMessageParcel(std::vector<USBConfig> &configs, MessageParcel &data)
{
    for (auto it = configs.begin(); it != configs.end(); ++it) {
        USBConfig config = *it;
        WRITE_PARCEL_WITH_RET(data, Int32, config.GetId(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, config.GetAttributes(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, config.GetMaxPower(), UEC_SERVICE_WRITE_PARCEL_ERROR);

        WRITE_PARCEL_WITH_RET(data, Uint8, config.GetiConfiguration(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, String, config.GetName(), UEC_SERVICE_WRITE_PARCEL_ERROR);

        WRITE_PARCEL_WITH_RET(data, Int32, config.GetInterfaceCount(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, config.ToString().c_str());
        int32_t ret = SetDeviceInterfacesMessageParcel(config.GetInterfaces(), data);
        if (ret) {
            return ret;
        }
    }
    return UEC_OK;
}

int32_t UsbServerStub::SetDeviceInterfacesMessageParcel(std::vector<UsbInterface> &interfaces, MessageParcel &data)
{
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
        UsbInterface interface = *it;
        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetId(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetClass(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetSubClass(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetAlternateSetting(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetProtocol(), UEC_SERVICE_WRITE_PARCEL_ERROR);

        WRITE_PARCEL_WITH_RET(data, Uint8, interface.GetiInterface(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, String, interface.GetName(), UEC_SERVICE_WRITE_PARCEL_ERROR);

        WRITE_PARCEL_WITH_RET(data, Int32, interface.GetEndpointCount(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, interface.ToString().c_str());
        int32_t ret = SetDeviceEndpointsMessageParcel(interface.GetEndpoints(), data);
        if (ret) {
            return ret;
        }
    }
    return UEC_OK;
}

int32_t UsbServerStub::SetDeviceEndpointsMessageParcel(std::vector<USBEndpoint> &eps, MessageParcel &data)
{
    for (auto it = eps.begin(); it != eps.end(); ++it) {
        USBEndpoint ep = *it;
        WRITE_PARCEL_WITH_RET(data, Int32, ep.GetAddress(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, ep.GetAttributes(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, ep.GetInterval(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        WRITE_PARCEL_WITH_RET(data, Int32, ep.GetMaxPacketSize(), UEC_SERVICE_WRITE_PARCEL_ERROR);
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s devInfo=%{public}s", __func__, ep.ToString().c_str());
    }
    return UEC_OK;
}
} // namespace USB
} // namespace OHOS
