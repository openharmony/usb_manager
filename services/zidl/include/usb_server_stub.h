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

#ifndef USBMGR_USB_SERVER_STUB_H
#define USBMGR_USB_SERVER_STUB_H

#include <vector>
#include "iremote_stub.h"
#include "iusb_srv.h"
#include "nocopyable.h"
#include "usb_device.h"

namespace OHOS {
namespace USB {
class UsbServerStub : public IRemoteStub<IUsbSrv> {
public:
    DISALLOW_COPY_AND_MOVE(UsbServerStub);

    UsbServerStub() = default;
    virtual ~UsbServerStub() = default;

    int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;

private:
    bool StubHost(uint32_t code, int32_t &result, MessageParcel &data, MessageParcel &reply, MessageOption &option);
    bool StubDevice(uint32_t code, int32_t &result, MessageParcel &data, MessageParcel &reply, MessageOption &option);

    int32_t DoGetCurrentFunctions(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoSetCurrentFunctions(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoUsbFunctionsFromString(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoUsbFunctionsToString(MessageParcel &data, MessageParcel &reply, MessageOption &option);

    int32_t DoOpenDevice(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoHasRight(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoRequestRight(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoRemoveRight(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetPorts(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetSupportedModes(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoSetPortRole(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoClaimInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoReleaseInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoBulkTransferRead(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoBulkTransferWrite(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoControlTransfer(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoSetActiveConfig(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetActiveConfig(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoSetInterface(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetRawDescriptor(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetFileDescriptor(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoRequestQueue(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoRequestWait(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoRequestCancel(MessageParcel &data, MessageParcel &reply, MessageOption &option);

    int32_t DoClose(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoGetDevices(MessageParcel &data, MessageParcel &reply, MessageOption &option);

    int32_t WriteUsbPort(MessageParcel &reply, const UsbPort &port);

    int32_t GetDeviceMessage(MessageParcel &data, uint8_t &busNum, uint8_t &devAddr);
    int32_t SetBufferMessage(MessageParcel &data, const std::vector<uint8_t> &bufferData);
    int32_t GetBufferMessage(MessageParcel &data, std::vector<uint8_t> &bufferData);
    int32_t SetDeviceListMessageParcel(std::vector<UsbDevice> &deviceList, MessageParcel &data);
    int32_t SetDeviceMessageParcel(UsbDevice &devInfo, MessageParcel &data);
    int32_t SetDeviceConfigsMessageParcel(std::vector<USBConfig> &configs, MessageParcel &data);
    int32_t SetDeviceInterfacesMessageParcel(std::vector<UsbInterface> &interfaces, MessageParcel &data);
    int32_t SetDeviceEndpointsMessageParcel(std::vector<USBEndpoint> &eps, MessageParcel &data);

    int32_t DoRegBulkCallback(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoUnRegBulkCallback(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoBulkRead(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoBulkWrite(MessageParcel &data, MessageParcel &reply, MessageOption &option);
    int32_t DoBulkCancel(MessageParcel &data, MessageParcel &reply, MessageOption &option);
};
} // namespace USB
} // namespace OHOS

#endif // USBMGR_USB_SERVER_STUB_H
