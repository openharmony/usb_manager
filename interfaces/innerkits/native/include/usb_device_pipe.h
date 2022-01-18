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

#ifndef USB_PIPE_H
#define USB_PIPE_H

#include "iusb_srv.h"

namespace OHOS {
namespace USB {
class USBDevicePipe {
public:
    USBDevicePipe();
    ~USBDevicePipe() {};
    USBDevicePipe(uint8_t busNum, uint8_t devAddr);
    int32_t ClaimInterface(const UsbInterface &interface, bool force);
    int32_t ReleaseInterface(const UsbInterface &interface);
    int32_t BulkTransfer(const USBEndpoint &endpoint, std::vector<uint8_t> &bufferData, int32_t timeOut);
    int32_t ControlTransfer(const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &bufferData);
    int32_t SetConfiguration(const USBConfig &config);
    int32_t SetInterface(const UsbInterface &interface);
    int32_t Close();

    void SetBusNum(uint8_t busNum);
    void SetDevAddr(uint8_t devAddr);
    uint8_t GetBusNum() const;
    uint8_t GetDevAddr() const;

private:
    uint8_t busNum;
    uint8_t devAddr;
    void *usbClient;
};
} // namespace USB
} // namespace OHOS

#endif // USB_PIPE_H
