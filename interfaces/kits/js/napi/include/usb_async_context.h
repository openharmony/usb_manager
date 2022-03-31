/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
const int32_t NONE = 0;
const int32_t SOURCE = 1;
const int32_t SINK = 2;

const int32_t HOST = 1;
const int32_t DEVICE = 2;

const int32_t UFP = 1;
const int32_t DFP = 2;
const int32_t DRP = 3;
const int32_t NUM_MODES = 4;

const int32_t USB_REQUEST_TARGET_DEVICE = 0;
const int32_t USB_REQUEST_TARGET_INTERFACE = 1;
const int32_t USB_REQUEST_TARGET_ENDPOINT = 2;
const int32_t USB_REQUEST_TARGET_OTHER = 3;

const int32_t USB_REQUEST_TYPE_STANDARD = 0;
const int32_t USB_REQUEST_TYPE_CLASS = 1;
const int32_t USB_REQUEST_TYPE_VENDOR = 2;

const int32_t USB_REQUEST_DIR_TO_DEVICE = 0;
const int32_t USB_REQUEST_DIR_FROM_DEVICE = 0x80;

const int32_t ACM = 1;
const int32_t ECM = 2;
const int32_t HDC = 4;
const int32_t MTP = 8;
const int32_t PTP = 16;
const int32_t RNDIS = 32;
const int32_t MIDI = 64;
const int32_t AUDIO_SOURCE = 128;
const int32_t NCM = 256;

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
    uint32_t dataSize;
    int32_t timeOut = 0;
};

struct USBBulkTransferAsyncContext : USBAsyncContext {
    uint8_t *buffer;
    uint32_t bufferLength;
    uint32_t dataSize;
    int32_t timeOut = 0;
    USBDevicePipe pipe;
    USBEndpoint endpoint;
};
} // namespace USB
} // namespace OHOS
#endif // USB_ASYNC_CONTEXT_H
