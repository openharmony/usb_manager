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

#include "usb_device_pipe.h"
#include "iusb_srv.h"
#include "usb_common.h"
#include "usb_device.h"
#include "usb_errors.h"
#include "usb_request.h"
#include "usb_srv_client.h"

namespace OHOS {
namespace USB {
USBDevicePipe::USBDevicePipe() : usbClient(&UsbSrvClient::GetInstance()) {}
USBDevicePipe::USBDevicePipe(uint8_t busNum, uint8_t devAddr)
    : busNum(busNum), devAddr(devAddr), usbClient(&UsbSrvClient::GetInstance())
{
}

int32_t USBDevicePipe::Close()
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    int32_t ret = ERR_OK;
    bool bSuccess = ((UsbSrvClient *)usbClient)->Close(*this);
    if (!bSuccess) {
        ret = ERR_INVALID_VALUE;
    }
    return ret;
}
int32_t USBDevicePipe::ClaimInterface(const UsbInterface &interface, bool force)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->ClaimInterface(*this, interface, force);
}
int32_t USBDevicePipe::ReleaseInterface(const UsbInterface &interface)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->ReleaseInterface(*this, interface);
}

int32_t USBDevicePipe::BulkTransfer(const USBEndpoint &endpoint, std::vector<uint8_t> &vdata, int32_t timeout)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->BulkTransfer(*this, endpoint, vdata, timeout);
}

int32_t USBDevicePipe::ControlTransfer(const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &vdata)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->ControlTransfer(*this, ctrl, vdata);
}
int32_t USBDevicePipe::SetConfiguration(const USBConfig &config)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->SetConfiguration(*this, config);
}
int32_t USBDevicePipe::SetInterface(const UsbInterface &interface)
{
    RETURN_IF_WITH_RET(usbClient == nullptr, ERR_NO_INIT);
    return ((UsbSrvClient *)usbClient)->SetInterface(*this, interface);
}
void USBDevicePipe::SetBusNum(uint8_t busNum)
{
    this->busNum = busNum;
}
void USBDevicePipe::SetDevAddr(uint8_t devAddr)
{
    this->devAddr = devAddr;
}
uint8_t USBDevicePipe::GetBusNum() const
{
    return busNum;
}
uint8_t USBDevicePipe::GetDevAddr() const
{
    return devAddr;
}
} // namespace USB
} // namespace OHOS
