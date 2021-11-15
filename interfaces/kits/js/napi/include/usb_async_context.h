/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef USB_ASYNC_CONTEXT_H
#define USB_ASYNC_CONTEXT_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "usb_device_pipe.h"
#include "usb_endpoint.h"
#include "usb_request.h"

namespace OHOS {
namespace USB {
struct USBAsyncContext {
    napi_env env;
    napi_async_work work;

    napi_deferred deferred;
    napi_status status;
};

struct USBRightAsyncContext : USBAsyncContext {
    std::string deviceName;
    bool hasRight = false;
};

struct USBFunctionAsyncContext : USBAsyncContext {
    int32_t functions;
};

struct USBPortRoleAsyncContext : USBAsyncContext {
    int32_t portId;
    int32_t powerRole;
    int32_t dataRole;
};

struct USBControlTransferAsyncContext : USBAsyncContext {
    USBDevicePipe pipe;
    int32_t request;
    int32_t target;
    int32_t reqType;
    int32_t directon;
    int32_t value;
    int32_t index;
    uint8_t *buffer;
    uint32_t bufferLength;
    int32_t timeout = 0;
};

struct USBBulkTransferAsyncContext : USBAsyncContext {
    uint8_t *buffer;
    uint32_t bufferLength;
    int32_t timeout = 0;
    USBDevicePipe pipe;
    USBEndpoint endpoint;
};

struct USBQueueAsyncContext : USBAsyncContext {
    UsbRequest req;
    uint8_t *buffer;
    uint32_t bufferLength;
};
} // namespace USB
} // namespace OHOS
#endif // USB_ASYNC_CONTEXT_H