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

#ifndef USBD_CLIENT_H
#define USBD_CLIENT_H

#include "usbd_subscriber.h"
#include "usb_param.h"

namespace OHOS {
namespace USB {
class UsbdClient {
public:
    /* usb主设备接口 */
    static int32_t OpenDevice(const UsbDev &dev);
    static int32_t CloseDevice(const UsbDev &dev);
    static int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &decriptor);
    static int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &decriptor);
    static int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &decriptor);
    static int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &decriptor);
    static int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
    static int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
    static int32_t ClaimInterface(const UsbDev &dev, uint8_t interfaceid);
    static int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceid);
    static int32_t SetInterface(const UsbDev &dev, uint8_t interfaceid, uint8_t altIndex);
    static int32_t
        BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, std::vector<uint8_t> &data);
    static int32_t
        BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, const std::vector<uint8_t> &data);
    static int32_t ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
    static int32_t
        InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, std::vector<uint8_t> &data);
    static int32_t
        InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, std::vector<uint8_t> &data);
    static int32_t IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, std::vector<uint8_t> &data);
    static int32_t
        IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout, std::vector<uint8_t> &data);
    static int32_t RequestQueue(const UsbDev &dev,
                                const UsbPipe &pipe,
                                const std::vector<uint8_t> &clientData,
                                const std::vector<uint8_t> &buffer);
    static int32_t
        RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer, int32_t timeout);
    static int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);

    /* usb从设备接口 */
    static int32_t GetCurrentFunctions(int32_t &funcs);
    static int32_t SetCurrentFunctions(int32_t funcs);
    static int32_t SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole);
    static int32_t QueryPort(int32_t &portId, int32_t &powerRole, int32_t &dataRole, int32_t &mode);

    static ErrCode BindUsbdSubscriber(const sptr<UsbdSubscriber> &subscriber);
    static ErrCode UnbindUsbdSubscriber();

private:
    static void PrintBuffer(const char *title, const uint8_t *buffer, uint32_t length);
    static int32_t SetDeviceMessage(MessageParcel &data, const UsbDev &dev);
    static int32_t SetBufferMessage(MessageParcel &data, const std::vector<uint8_t> &tdata);
    static int32_t GetBufferMessage(MessageParcel &data, std::vector<uint8_t> &tdata);
    static sptr<IRemoteObject> GetUsbdService();
    static ErrCode DoDispatch(uint32_t cmd, MessageParcel &data, MessageParcel &reply);
};
} // namespace USB
} // namespace OHOS
#endif // USBD_CLIENT_H
