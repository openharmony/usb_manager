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

#ifndef USB_ENDPOINT_H
#define USB_ENDPOINT_H

#include <iostream>
#include <sstream>
#include <string>
#include "usb_common.h"

namespace OHOS {
namespace USB {
class USBEndpoint {
public:
    USBEndpoint(uint32_t address, uint32_t attributes, uint32_t interval, uint32_t maxPacketSize)
    {
        this->address = address;
        this->attributes = attributes;
        this->interval = interval;
        this->maxPacketSize = maxPacketSize;
    }
    USBEndpoint() {}
    ~USBEndpoint() {}

    uint8_t GetNumber() const
    {
        return address & USB_ENDPOINT_NUMBER_MASK;
    }
    const int32_t &GetAddress() const
    {
        return address;
    }

    int32_t GetDirection() const
    {
        return address & USB_ENDPOINT_DIR_MASK;
    }

    const int32_t &GetAttributes() const
    {
        return attributes;
    }

    int32_t GetEndpointNumber() const
    {
        return address & USB_ENDPOINT_NUMBER_MASK;
    }

    const int32_t &GetInterval() const
    {
        return interval;
    }

    const int32_t &GetMaxPacketSize() const
    {
        return maxPacketSize;
    }

    int32_t GetType() const
    {
        return (attributes & USB_ENDPOINT_XFERTYPE_MASK);
    }

    std::string ToString() const
    {
        std::string ret = "USBEndpoint:[Address:";
        ret.append(std::to_string(address))
            .append(", Direction:")
            .append(std::to_string(GetDirection()))
            .append(", Attributes:")
            .append(std::to_string(attributes))
            .append(", EndpointNumber:")
            .append(std::to_string(GetEndpointNumber()))
            .append(", Interval:")
            .append(std::to_string(interval))
            .append(", MaxPacketSize:")
            .append(std::to_string(maxPacketSize))
            .append(", Type:")
            .append(std::to_string(GetType()))
            .append("]");
        return ret;
    }
    void SetAddr(int32_t val)
    {
        address = val;
    }
    void SetAttr(int32_t val)
    {
        attributes = val;
    }
    void SetInterval(int32_t val)
    {
        interval = val;
    }
    void SetMaxPacketSize(int32_t val)
    {
        maxPacketSize = val;
    }
    void SetInterfaceId(uint8_t interfaceId)
    {
        this->interfaceId = interfaceId;
    }
    int8_t GetInterfaceId() const
    {
        return interfaceId;
    }

private:
    int32_t address = INVALID_USB_INT_VALUE;
    int32_t attributes = INVALID_USB_INT_VALUE;
    int32_t interval = INVALID_USB_INT_VALUE;
    int32_t maxPacketSize = INVALID_USB_INT_VALUE;
    uint8_t interfaceId;
};
} // namespace USB
} // namespace OHOS

#endif // USB_ENDPOINT_H
