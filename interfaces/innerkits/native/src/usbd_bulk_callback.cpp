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

#include "usbd_bulk_callback.h"

#include "hilog_wrapper.h"
#include "usb_errors.h"

namespace OHOS::USB {
int32_t UsbdBulkCallBack::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    switch (code) {
        case CMD_USBD_BULK_CALLBACK_WRITE: {
            if (data.ReadInterfaceToken() != GetObjectDescriptor()) {
                USB_HILOGE(MODULE_USB_INNERKIT, "check interface token failed, code=%{public}u", code);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }

            int32_t status;
            int32_t actLength;
            if (!data.ReadInt32(status)) {
                USB_HILOGE(MODULE_USB_INNERKIT, "get status error");
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            if (!data.ReadInt32(actLength)) {
                USB_HILOGE(MODULE_USB_INNERKIT, "get actLength error");
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }

            USB_HILOGI(MODULE_USB_INNERKIT, "status:%{public}d actLength:%{public}d", status, actLength);
            OnBulkWriteCallback(status, actLength);
            break;
        }
        case CMD_USBD_BULK_CALLBACK_READ: {
            if (data.ReadInterfaceToken() != GetObjectDescriptor()) {
                USB_HILOGE(MODULE_USB_INNERKIT, "check interface token failed, code=%{public}u", code);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }

            int32_t status;
            int32_t actLength;
            if (!data.ReadInt32(status)) {
                USB_HILOGE(MODULE_USB_INNERKIT, "get status error code=%{public}u", code);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            if (!data.ReadInt32(actLength)) {
                USB_HILOGE(MODULE_USB_INNERKIT, "get actLength error code=%{public}u", code);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }

            USB_HILOGI(MODULE_USB_INNERKIT, "%{public}d status:%{public}d actLength:%{public}d", __LINE__, status,
                actLength);
            OnBulkReadCallback(status, actLength);
            break;
        }
        default: {
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
    return UEC_OK;
}
} // namespace OHOS::USB
