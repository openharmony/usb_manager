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
    USBConfig(uint32_t id, uint32_t attributes, std::string name, uint32_t maxPower,
        std::vector<UsbInterface> interfaces)
    {
        this->id_ = id;
        this->attributes_ = attributes;
        this->maxPower_ = maxPower;
        this->name_ = name;
        this->interfaces_ = interfaces;
    }

    explicit USBConfig(const Json::Value &config)
    {
        id_ = config["id"].asInt();
        attributes_ = config["attributes"].asUInt();
        maxPower_ = config["maxPower"].asInt();
        name_ = config["name"].asString();

        Json::Value interfaces = config["interfaces"];
        for (uint32_t idx = 0; idx < interfaces.size(); ++idx) {
            interfaces_.emplace_back(interfaces[idx]);
        }
    }

    USBConfig() {}
    ~USBConfig() {}

    const int32_t &GetId() const
    {
        return id_;
    }

    const uint32_t &GetAttributes() const
    {
        return attributes_;
    }

    bool GetInterface(uint32_t index, UsbInterface &interface) const
    {
        if (index >= interfaces_.size()) {
            return false;
        }
        interface = interfaces_[index];
        return true;
    }

    uint32_t GetInterfaceCount() const
    {
        return interfaces_.size();
    }

    int32_t GetMaxPower() const
    {
        // 2 represent maxPower units
        return maxPower_ * 2;
    }

    const std::string &GetName() const
    {
        return name_;
    }

    bool IsRemoteWakeup() const
    {
        return (attributes_ & USB_CFG_REMOTE_WAKEUP) != 0;
    }

    bool IsSelfPowered() const
    {
        return (attributes_ & USB_CFG_SELF_POWERED) != 0;
    }

    void SetInterfaces(const std::vector<UsbInterface> &interfaces)
    {
        this->interfaces_ = interfaces;
    }

    std::vector<UsbInterface> &GetInterfaces()
    {
        return interfaces_;
    }

    void SetId(int32_t id)
    {
        this->id_ = id;
    }

    void SetAttribute(uint32_t attributes)
    {
        this->attributes_ = attributes;
    }

    void SetMaxPower(int32_t maxPower)
    {
        this->maxPower_ = maxPower;
    }

    std::string ToString() const
    {
        std::ostringstream ss;
        ss << "name=" << name_ << ","
           << "id=" << id_ << ","
           << "iConfiguration=" << (int32_t)iConfiguration_ << ","
           << "attributes=" << attributes_ << ","
           << "maxPower=" << maxPower_ << ";  ";
        std::string str = "USBConfig[" + ss.str() + "]";
        ss.str("");
        for (size_t i = 0; i < interfaces_.size(); ++i) {
            const UsbInterface &interface = interfaces_[i];
            str += interface.ToString();
        }
        return str;
    }

    void SetName(const std::string &name)
    {
        this->name_ = name;
    }

    void SetiConfiguration(uint8_t idx)
    {
        this->iConfiguration_ = idx;
    }

    uint8_t GetiConfiguration()
    {
        return this->iConfiguration_;
    }

    Json::Value ToJson() const
    {
        Json::Value config;
        config["id"] = id_;
        config["attributes"] = attributes_;
        config["maxPower"] = maxPower_;
        config["name"] = name_;
        config["isRemoteWakeup"] = IsRemoteWakeup();
        config["isSelfPowered"] = IsSelfPowered();

        Json::Value interfaces;
        for (auto &intf : interfaces_) {
            interfaces.append(intf.ToJson());
        }
        config["interfaces"] = interfaces;

        return config;
    }

private:
    int32_t id_ = INVALID_USB_INT_VALUE;
    uint32_t attributes_ = 0;
    std::vector<UsbInterface> interfaces_;
    int32_t maxPower_ = INVALID_USB_INT_VALUE;
    std::string name_;
    uint8_t iConfiguration_ = UINT8_MAX;
};
} // namespace USB
} // namespace OHOS

#endif // USB_CONFIG_H
