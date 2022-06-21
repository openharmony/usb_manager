/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "usb_common_fuzz.h"

#include "usb_errors.h"
#include "usb_srv_client.h"

namespace OHOS {
namespace USB {
std::tuple<bool, const USBDevicePipe&, const UsbInterface&> UsbMgrPrepareFuzzEnv()
{
    std::vector<UsbDevice> devList;
    auto &usbSrvClient = UsbSrvClient::GetInstance();
    auto ret = usbSrvClient.GetDevices(devList);
    if (ret != UEC_OK || devList.empty()) {
        USB_HILOGE(MODULE_USB_SERVICE, "get devices failed ret=%{public}d", ret);
        return {false, {}, {}};
    }

    USBDevicePipe pipe;
    UsbDevice device = devList.front();
    usbSrvClient.RequestRight(device.GetName());
    ret = usbSrvClient.OpenDevice(device, pipe);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "open device failed ret=%{public}d", ret);
        return {false, {}, {}};
    }

    UsbInterface interface = devList.front().GetConfigs().front().GetInterfaces().at(1);
    ret = usbSrvClient.ClaimInterface(pipe, interface, true);
    if (ret != UEC_OK) {
        USB_HILOGE(MODULE_USB_SERVICE, "ClaimInterface failed ret=%{public}d", ret);
        usbSrvClient.Close(pipe);
        return {false, {}, {}};
    }

    return {true, pipe, interface};
}
} // USB
} // OHOS
