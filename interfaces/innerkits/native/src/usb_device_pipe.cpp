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

#include "usb_device_pipe.h"
#include "iusb_srv.h"
#include "usb_common.h"
#include "usb_device.h"
#include "usb_errors.h"
#include "usb_request.h"
#include "usb_srv_client.h"

namespace OHOS {
namespace USB {
USBDevicePipe::USBDevicePipe() {}
USBDevicePipe::USBDevicePipe(uint8_t busNum, uint8_t devAddr)
    : busNum(busNum), devAddr(devAddr)
{
}

int32_t USBDevicePipe::Close()
{
    int32_t ret = ERR_OK;
    bool bSuccess = UsbSrvClient::GetInstance().Close(*this);
    if (!bSuccess) {
        ret = ERR_INVALID_VALUE;
    }
    return ret;
}

int32_t USBDevicePipe::ClaimInterface(const UsbInterface &interface, bool force)
{
    return UsbSrvClient::GetInstance().ClaimInterface(*this, interface, force);
}

int32_t USBDevicePipe::ReleaseInterface(const UsbInterface &interface)
{
    return UsbSrvClient::GetInstance().ReleaseInterface(*this, interface);
}

int32_t USBDevicePipe::BulkTransfer(const USBEndpoint &endpoint, std::vector<uint8_t> &bufferData, int32_t timeOut)
{
    return UsbSrvClient::GetInstance().BulkTransfer(*this, endpoint, bufferData, timeOut);
}

int32_t USBDevicePipe::ControlTransfer(const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &bufferData)
{
    return UsbSrvClient::GetInstance().ControlTransfer(*this, ctrl, bufferData);
}

int32_t USBDevicePipe::SetConfiguration(const USBConfig &config)
{
    return UsbSrvClient::GetInstance().SetConfiguration(*this, config);
}

int32_t USBDevicePipe::SetInterface(const UsbInterface &interface)
{
    return UsbSrvClient::GetInstance().SetInterface(*this, interface);
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
