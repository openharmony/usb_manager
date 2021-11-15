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

#ifndef USB_CONFIG_H
#define USB_CONFIG_H

#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>
#include "usb_common.h"
#include "usb_interface.h"

namespace OHOS {
namespace USB {
class USBConfig {
public:
    USBConfig(uint32_t id,
              uint32_t attributes,
              std::string name,
              uint32_t maxPower,
              std::vector<UsbInterface> interfaces)
    {
        this->id = id;
        this->attributes = attributes;
        this->maxPower = maxPower;
        this->name = name;
        this->interfaces = interfaces;
    }
    USBConfig() {}
    ~USBConfig() {}

    const int32_t &GetId() const
    {
        return id;
    }

    const int32_t &GetAttributes() const
    {
        return attributes;
    }

    bool GetInterface(uint32_t index, UsbInterface &interface) const
    {
        if (index >= interfaces.size()) {
            return false;
        }
        interface = interfaces[index];
        return true;
    }

    int32_t GetInterfaceCount() const
    {
        return interfaces.size();
    }

    int32_t GetMaxPower() const
    {
        return maxPower * 2;
    }

    const std::string &GetName() const
    {
        return name;
    }

    bool IsRemoteWakeup() const
    {
        return (attributes & USB_CFG_REMOTE_WAKEUP) != 0;
    }

    bool IsSelfPowered() const
    {
        return (attributes & USB_CFG_SELF_POWERED) != 0;
    }

    void SetInterfaces(const std::vector<UsbInterface> &interfaces)
    {
        this->interfaces = interfaces;
    }
    std::vector<UsbInterface> &GetInterfaces()
    {
        return interfaces;
    }
    void SetId(int32_t id)
    {
        this->id = id;
    }
    void SetAttribute(int32_t attributes)
    {
        this->attributes = attributes;
    }
    void SetMaxPower(int32_t maxPower)
    {
        this->maxPower = maxPower;
    }
    std::string ToString() const
    {
        std::string str;
        std::ostringstream ss;
        ss << "name=" << name << ","
           << "id=" << id << ","
           << "iConfiguration=" << (int32_t)iConfiguration << ","
           << "attributes=" << attributes << ","
           << "maxPower=" << maxPower << ";  ";
        str = ss.str();
        ss.str("");
        for (size_t i = 0; i < interfaces.size(); ++i) {
            const UsbInterface &interface = interfaces[i];
            str += interface.ToString();
        }
        return str;
    }
    void SetName(std::string Name)
    {
        this->name = Name;
    }
    void SetiConfiguration(uint8_t idx)
    {
        this->iConfiguration = idx;
    }
    uint8_t GetiConfiguration()
    {
        return this->iConfiguration;
    }

private:
    int32_t id = INVALID_USB_INT_VALUE;
    int32_t attributes = INVALID_USB_INT_VALUE;
    std::vector<UsbInterface> interfaces;
    int32_t maxPower = INVALID_USB_INT_VALUE;
    std::string name;
    uint8_t iConfiguration;
};
} // namespace USB
} // namespace OHOS

#endif // USB_CONFIG_H
