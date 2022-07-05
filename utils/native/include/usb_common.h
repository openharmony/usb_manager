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

#ifndef USB_COMMON_H
#define USB_COMMON_H

#include <cstdint>
#include "hilog_wrapper.h"

namespace OHOS {
namespace USB {
/**
 * Invalid usb int value
 */
constexpr int32_t INVALID_USB_INT_VALUE = -1;

/**
 * Bitmask used for extracting the USBEndpoint type from it's address
 */
constexpr uint32_t USB_ENDPOINT_XFERTYPE_MASK = 0x03;

/**
 * Control USBEndpoint type
 */
constexpr int32_t USB_ENDPOINT_XFER_CONTROL = 0;

/**
 * Isochronous USBEndpoint type
 */
constexpr int32_t USB_ENDPOINT_XFER_ISOC = 1;

/**
 * Bulk USBEndpoint type
 */
constexpr int32_t USB_ENDPOINT_XFER_BULK = 2;

/**
 * Interrupt USBEndpoint type
 */
constexpr int32_t USB_ENDPOINT_XFER_INT = 3;

/**
 * Bitmask used for extracting the USBEndpoint number from it's address
 */
constexpr uint32_t USB_ENDPOINT_NUMBER_MASK = 0x0f;

/**
 * Bitmask used for extracting the USBEndpoint direction from it's address
 */
constexpr uint32_t USB_ENDPOINT_DIR_MASK = 0x80;

/**
 * Used to signify direction of data for USBEndpoint is OUT, host to device
 */
constexpr uint32_t USB_ENDPOINT_DIR_OUT = 0;

/**
 * Used to signify direction of data for USBEndpoint is IN, device to host
 */
constexpr uint32_t USB_ENDPOINT_DIR_IN = 0x80;

/**
 * Bitmask for self power in the USBConfig
 */
constexpr uint32_t USB_CFG_SELF_POWERED = 0x80;

/**
 * Bitmask for remote wakeup in the USBConfig
 */
constexpr uint32_t USB_CFG_REMOTE_WAKEUP = 0x20;

#define INVALID_STRING_VALUE ("")
#define RETURN_IF_WITH_RET(cond, retval) \
    if (cond) {                          \
        return (retval);                 \
    }
#define RETURN_IF(cond) \
    if (cond) {         \
        return;         \
    }
#define RETURN_IF_WITH_LOG(cond, loginfo)                                    \
    do {                                                                     \
        if (cond) {                                                          \
            USB_HILOGE(MODULE_COMMON, "%{public}s " #loginfo " ", __func__); \
            return;                                                          \
        }                                                                    \
    } while (0)

#define READ_PARCEL_NO_RET(parcel, type, out)                                       \
    do {                                                                            \
        if (!(parcel).Read##type(out)) {                                            \
            USB_HILOGE(MODULE_COMMON, "%{public}s read " #out " failed", __func__); \
            return;                                                                 \
        }                                                                           \
    } while (0)

#define WRITE_PARCEL_NO_RET(parcel, type, data)                                       \
    do {                                                                              \
        if (!(parcel).Write##type(data)) {                                            \
            USB_HILOGE(MODULE_COMMON, "%{public}s write " #data " failed", __func__); \
            return;                                                                   \
        }                                                                             \
    } while (0)

#define READ_PARCEL_WITH_RET(parcel, type, out, retval)                             \
    do {                                                                            \
        if (!(parcel).Read##type(out)) {                                            \
            USB_HILOGE(MODULE_COMMON, "%{public}s read " #out " failed", __func__); \
            return (retval);                                                        \
        }                                                                           \
    } while (0)

#define WRITE_PARCEL_WITH_RET(parcel, type, data, retval)                             \
    do {                                                                              \
        if (!(parcel).Write##type(data)) {                                            \
            USB_HILOGE(MODULE_COMMON, "%{public}s write " #data " failed", __func__); \
            return (retval);                                                          \
        }                                                                             \
    } while (0)

template <typename E> constexpr auto ToUnderlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}
} // namespace USB
} // namespace OHOS

#endif // USB_COMMON_H
