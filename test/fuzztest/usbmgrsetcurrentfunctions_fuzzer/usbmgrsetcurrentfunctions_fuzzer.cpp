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

#include "usbmgrsetcurrentfunctions_fuzzer.h"

#include "usb_srv_client.h"
#include "usb_errors.h"

namespace {
    const int32_t MAX_FUNC_NUM = 6;
}

namespace OHOS {
namespace USB {
    bool UsbMgrSetCurrentFunctionsFuzzTest(const uint8_t* data, size_t /* size */)
    {
        auto &usbSrvClient = UsbSrvClient::GetInstance();
        int32_t func = reinterpret_cast<int32_t>(data);
        if (func <= MAX_FUNC_NUM) {
            func += MAX_FUNC_NUM;
        }

        if (usbSrvClient.SetCurrentFunctions(func) == UEC_OK) {
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
    OHOS::USB::UsbMgrSetCurrentFunctionsFuzzTest(data, size);
    return 0;
}

