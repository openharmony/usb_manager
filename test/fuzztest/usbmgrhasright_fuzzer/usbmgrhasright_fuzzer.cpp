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

#include "usbmgrhasright_fuzzer.h"

#include "usb_errors.h"
#include "usb_srv_client.h"

namespace OHOS {
namespace USB {
    bool UsbMgrHasRightFuzzTest(const uint8_t* data, size_t /* size */)
    {
        auto &usbSrvClient = UsbSrvClient::GetInstance();
        std::vector<UsbDevice> devList;
        auto ret = usbSrvClient.GetDevices(devList);
        if (ret != UEC_OK || devList.empty()) {
            USB_HILOGE(MODULE_USB_SERVICE, "get devices failed");
            return false;
        }

        ret = usbSrvClient.RequestRight(devList[0].GetName());
        if (ret != UEC_OK) {
            USB_HILOGE(MODULE_USB_SERVICE, "request right failed");
            return false;
        }

        if (usbSrvClient.HasRight(std::string((const char*)data))) {
            return false;
        }
        return true;
    }
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::USB::UsbMgrHasRightFuzzTest(data, size);
    return 0;
}

