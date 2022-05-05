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

#include "usbmgrbulkcancel_fuzzer.h"

#include <array>
#include <optional>

#include "iremote_object.h"
#include "ashmem.h"
#include "usb_errors.h"
#include "usb_callback_test.h"
#include "usb_common_fuzz.h"
namespace {
const uint32_t ASHMEM_MAX_SIZE = 1024;
const uint32_t MEM_DATA = 1024 * 1024;
}
namespace OHOS {
namespace USB {
    sptr<Ashmem> GetSharedMem()
    {
        sptr<Ashmem> asmptr = Ashmem::CreateAshmem("ttashmem001", MEM_DATA);
        if (asmptr == nullptr) {
            USB_HILOGE(MODULE_USB_SERVICE, "InitAshmemOne CreateAshmem failed");
            return nullptr;
        }

        asmptr->MapReadAndWriteAshmem();

        std::array<uint8_t, ASHMEM_MAX_SIZE> tdata;
        tdata.fill('Y');
        uint32_t offset = 0;
        uint32_t tlen = 0;
        while (offset < MEM_DATA) {
            tlen = (MEM_DATA - offset) < ASHMEM_MAX_SIZE ? (MEM_DATA - offset) : ASHMEM_MAX_SIZE;
            asmptr->WriteToAshmem(tdata.data(), tlen, offset);
            offset += tlen;
        }
        return asmptr;
    }

    bool UsbMgrBulkCancelFuzzTest(const uint8_t* data, size_t /* size */)
    {
        auto[res, pipe, interface] = UsbMgrPrepareFuzzEnv();
        if (!res) {
            USB_HILOGE(MODULE_USB_SERVICE, "prepare error");
            return false;
        }

        auto& usbSrvClient = UsbSrvClient::GetInstance();
        sptr<UsbCallbackTest> cb = new UsbCallbackTest();
        USBEndpoint point = interface.GetEndpoints().at(1);
        auto ret = usbSrvClient.RegBulkCallback(pipe, point, cb);
        if (ret != UEC_OK) {
            USB_HILOGE(MODULE_USB_SERVICE, "RegBulkCallback failed ret=%{public}d", ret);
            return false;
        }

        auto sharedMem = GetSharedMem();
        if (sharedMem == nullptr) {
            USB_HILOGE(MODULE_USB_SERVICE, "GetSharedMem failed ret=%{public}d", ret);
            return false;
        }

        ret = usbSrvClient.BulkWrite(pipe, point, sharedMem);
        if (ret != UEC_OK) {
            USB_HILOGE(MODULE_USB_SERVICE, "BulkWrite failed ret=%{public}d", ret);
            return false;
        }

        if (usbSrvClient.BulkCancel(reinterpret_cast<const USBDevicePipe&>(data),
            reinterpret_cast<const USBEndpoint&>(data)) == UEC_OK) {
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
    OHOS::USB::UsbMgrBulkCancelFuzzTest(data, size);
    return 0;
}
