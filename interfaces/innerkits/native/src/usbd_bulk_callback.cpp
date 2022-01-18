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
#include <hdf_base.h>
#include <hdf_log.h>
#include <hdf_sbuf_ipc.h>
#include "usb_errors.h"

#define HDF_LOG_TAG USBD_BULK_CALLBACK

namespace OHOS::USB {
int32_t UsbdBulkCallBack::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    HDF_LOGI("%{public}s:%{public}d OnRemoteRequest entry! code:%{public}d", __func__, __LINE__, code);
    switch (code) {
        case CMD_USBD_BULK_CALLBACK_WRITE: {
            int32_t status;
            int32_t actLength;
            if (!data.ReadInt32(status)) {
                HDF_LOGE("%{public}s:%{public}d get status error", __func__, __LINE__);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            if (!data.ReadInt32(actLength)) {
                HDF_LOGE("%{public}s:%{public}d get actLength error", __func__, __LINE__);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            HDF_LOGI("%{public}s:%{public}d status:%{public}d actLength:%{public}d", __func__, __LINE__, status,
                     actLength);
            OnBulkWriteCallback(status, actLength);
            break;
        }
        case CMD_USBD_BULK_CALLBACK_READ: {
            int32_t status;
            int32_t actLength;
            if (!data.ReadInt32(status)) {
                HDF_LOGE("%{public}s:%{public}d get status error", __func__, __LINE__);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            if (!data.ReadInt32(actLength)) {
                HDF_LOGE("%{public}s:%{public}d get actLength error", __func__, __LINE__);
                return UEC_SERVICE_WRITE_PARCEL_ERROR;
            }
            HDF_LOGI("%{public}s:%{public}d status:%{public}d actLength:%{public}d", __func__, __LINE__, status,
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
