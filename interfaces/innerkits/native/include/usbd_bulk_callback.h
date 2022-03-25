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

#ifndef USBD_BULK_CALLBACK_H
#define USBD_BULK_CALLBACK_H

#include "ipc_object_stub.h"

namespace OHOS::USB {
class UsbdBulkCallBack : public OHOS::IPCObjectStub {
public:
    enum {
        CMD_USBD_BULK_CALLBACK_READ,
        CMD_USBD_BULK_CALLBACK_WRITE,
    };

    explicit UsbdBulkCallBack() : OHOS::IPCObjectStub(u"UsbdBulkCallback.V1_0") {}
    virtual ~UsbdBulkCallBack() = default;
    int32_t OnRemoteRequest(uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply,
        OHOS::MessageOption &option) override;
    virtual int32_t OnBulkWriteCallback(int32_t status, int32_t actLength) = 0;
    virtual int32_t OnBulkReadCallback(int32_t status, int32_t actLength) = 0;
};
} // namespace OHOS::USB
#endif
