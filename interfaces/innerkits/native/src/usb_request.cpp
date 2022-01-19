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

#include "usb_request.h"
#include "usb_common.h"
#include "usb_srv_client.h"

namespace OHOS {
namespace USB {
UsbRequest::UsbRequest() : usbClient((uint8_t *)(&UsbSrvClient::GetInstance())) {}
int32_t UsbRequest::Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
{
    this->pipe = pipe;
    this->endpoint = endpoint;
    int32_t ret = ((UsbSrvClient *)usbClient)->RequestInitialize(*this);
    if (ERR_OK != ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbRequest::%{public}s:%{public}d failed width ret = %{public}d.", __func__,
                   __LINE__, ret);
    }
    return ret;
}

int32_t UsbRequest::Queue()
{
    int32_t ret = ((UsbSrvClient *)usbClient)->RequestQueue(*this);
    if (ERR_OK != ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbRequest::%{public}s:%{public}d failed width ret = %{public}d.", __func__,
                   __LINE__, ret);
    }
    return ret;
}

int32_t UsbRequest::Free()
{
    int32_t ret = ((UsbSrvClient *)usbClient)->RequestFree(*this);
    if (ERR_OK != ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbRequest::%{public}s:%{public}d failed width ret = %{public}d.", __func__,
                   __LINE__, ret);
    }
    return ret;
}

int32_t UsbRequest::Abort()
{
    int32_t ret = ((UsbSrvClient *)usbClient)->RequestAbort(*this);
    if (ERR_OK != ret) {
        USB_HILOGE(MODULE_USB_INNERKIT, "UsbRequest::%{public}s:%{public}d failed width ret = %{public}d.", __func__,
                   __LINE__, ret);
    }
    return ret;
}
} // namespace USB
} // namespace OHOS
