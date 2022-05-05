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

#include "usbmgrbulkread_fuzzer.h"

#include "iremote_object.h"
#include "usb_errors.h"
#include "usb_callback_test.h"
#include "usb_common_fuzz.h"

namespace OHOS {
namespace USB {
    bool UsbMgrBulkReadFuzzTest(const uint8_t* data, size_t /* size */)
    {
        auto[res, pipe, interface] = UsbMgrPrepareFuzzEnv();
        if (!res) {
            USB_HILOGE(MODULE_USB_SERVICE, "prepare error");
            return false;
        }

        auto& usbSrvClient = UsbSrvClient::GetInstance();
        sptr<UsbCallbackTest> cb = new UsbCallbackTest();
        auto ret = usbSrvClient.RegBulkCallback(pipe, interface.GetEndpoints().at(1), cb);
        if (ret != UEC_OK) {
            USB_HILOGE(MODULE_USB_SERVICE, "RegBulkCallback failed ret=%{public}d", ret);
            return false;
        }

        if (usbSrvClient.BulkRead(reinterpret_cast<const USBDevicePipe&>(data),
            reinterpret_cast<const USBEndpoint&>(data), reinterpret_cast<sptr<Ashmem>&>(data)) == UEC_OK) {
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
    OHOS::USB::UsbMgrBulkReadFuzzTest(data, size);
    return 0;
}

