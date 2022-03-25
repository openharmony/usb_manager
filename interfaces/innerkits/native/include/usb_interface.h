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

#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "usb_endpoint.h"

namespace OHOS {
namespace USB {
class UsbInterface {
public:
    UsbInterface(int32_t mId,
                 int32_t mProtocol,
                 int32_t mClass,
                 int32_t mSubClass,
                 int32_t mAlternateSetting,
                 std::string mName,
                 std::vector<USBEndpoint> mEndpoints)
    {
        this->mId = mId;
        this->mProtocol = mProtocol;
        this->mClass = mClass;
        this->mSubClass = mSubClass;
        this->mAlternateSetting = mAlternateSetting;
        this->mEndpoints = mEndpoints;
    }

    UsbInterface() {}

    const std::string &GetName() const
    {
        return mName;
    }

    int32_t GetId() const
    {
        return mId;
    }

    int32_t GetClass() const
    {
        return mClass;
    }

    int32_t GetSubClass() const
    {
        return mSubClass;
    }

    int32_t GetAlternateSetting() const
    {
        return mAlternateSetting;
    }

    int32_t GetProtocol() const
    {
        return mProtocol;
    }

    int32_t GetEndpointCount() const
    {
        return mEndpoints.size();
    }

    void GetEndpoint(uint32_t index, USBEndpoint &ep) const
    {
        if (index < mEndpoints.size()) {
            ep = mEndpoints[index];
        }
    }

    std::vector<USBEndpoint> &GetEndpoints()
    {
        return mEndpoints;
    }

    void SetEndpoints(const std::vector<USBEndpoint> &eps)
    {
        mEndpoints = eps;
    }

    void SetId(int Val)
    {
        mId = Val;
    }

    void SetProtocol(int Val)
    {
        mProtocol = Val;
    }

    void SetClass(int Val)
    {
        mClass = Val;
    }

    void SetSubClass(int Val)
    {
        mSubClass = Val;
    }

    void SetAlternateSetting(int Val)
    {
        mAlternateSetting = Val;
    }

    void SetName(const std::string &Name)
    {
        mName = Name;
    }

    ~UsbInterface() {}

    std::string ToString() const
    {
        std::ostringstream ss;
        ss << "id=" << mId << ","
           << "mName=" << mName << ","
           << "iInterface=" << (int32_t)iInterface << ","
           << "mClass=" << mClass << ","
           << "mSubClass=" << mSubClass << ","
           << "mProtocol=" << mProtocol << ","
           << "mAlternateSetting=" << mAlternateSetting << "";
        std::string str = "UsbInterface[" + ss.str() + "];    ";
        ss.str("");
        for (size_t i = 0; i < mEndpoints.size(); ++i) {
            const USBEndpoint &endpoint = mEndpoints[i];
            str += endpoint.ToString();
        }
        return str;
    }

    void SetiInterface(uint8_t idx)
    {
        this->iInterface = idx;
    }

    uint8_t GetiInterface()
    {
        return this->iInterface;
    }

private:
    int mId;
    int mProtocol;
    int mClass;
    int mSubClass;
    int mAlternateSetting;
    std::string mName;
    std::vector<USBEndpoint> mEndpoints;
    uint8_t iInterface = UINT8_MAX;
};
} // namespace USB
} // namespace OHOS

#endif // USB_INTERFACE_H
