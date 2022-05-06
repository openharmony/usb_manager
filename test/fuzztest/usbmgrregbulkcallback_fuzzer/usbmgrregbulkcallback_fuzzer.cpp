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

#include "usbmgrregbulkcallback_fuzzer.h"

#include "iremote_object.h"
#include "usb_srv_client.h"
#include "usb_errors.h"
#include "usb_common_fuzz.h"

namespace OHOS {
namespace USB {
    bool UsbMgrRegBulkCallbackFuzzTest(const uint8_t* data, size_t /* size */)
    {
        auto[res, pipe, interface] = UsbMgrPrepareFuzzEnv();
        (void)pipe;
        (void)interface;
        if (!res) {
            USB_HILOGE(MODULE_USB_SERVICE, "prepare error");
            return false;
        }

        if (UsbSrvClient::GetInstance().RegBulkCallback(reinterpret_cast<const USBDevicePipe&>(data),
            reinterpret_cast<const USBEndpoint&>(data), reinterpret_cast<const sptr<IRemoteObject>&>(data)) == UEC_OK) {
            return false;
        }
        return true;
    }
} // USB
} // OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::USB::UsbMgrRegBulkCallbackFuzzTest(data, size);
    return 0;
}

