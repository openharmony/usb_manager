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

#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include <iostream>
#include <singleton.h>
#include <sstream>
#include <vector>
#include "iremote_object.h"
#include "usb_config.h"
#include "usb_interface.h"

namespace OHOS {
namespace USB {
class UsbDevice {
public:
    UsbDevice(std::string mName,
              std::string mManufacturerName,
              std::string mProductName,
              std::string mVersion,
              uint8_t devAddr,
              uint8_t busNum,
              int mVendorId,
              int mProductId,
              int mClass,
              int mSubclass,
              int mProtocol,
              std::vector<USBConfig> configs)
    {
        this->mName = mName;
        this->mManufacturerName = mManufacturerName;
        this->mProductName = mProductName;
        this->mVersion = mVersion;
        this->devAddr = devAddr;
        this->busNum = busNum;
        this->mVendorId = mVendorId;
        this->mProductId = mProductId;
        this->mClass = mClass;
        this->mSubclass = mSubclass;
        this->mProtocol = mProtocol;
        this->configs = configs;
    }
    UsbDevice() {}
    ~UsbDevice() {}
    const std::string &GetName() const
    {
        return mName;
    }
    const std::string &GetManufacturerName() const
    {
        return mManufacturerName;
    }
    const std::string &GetProductName() const
    {
        return mProductName;
    }
    const std::string &GetVersion() const
    {
        return mVersion;
    }
    int32_t GetVendorId() const
    {
        return mVendorId;
    }
    int32_t GetProductId() const
    {
        return mProductId;
    }
    int32_t GetClass() const
    {
        return mClass;
    }
    int32_t GetSubclass() const
    {
        return mSubclass;
    }
    int32_t GetProtocol() const
    {
        return mProtocol;
    }
    int32_t GetConfigCount() const
    {
        return configs.size();
    }
    int32_t GetConfig(uint32_t index, USBConfig &config) const
    {
        if (index >= configs.size()) {
            return ERR_INVALID_VALUE;
        }
        config = configs[index];
        return ERR_OK;
    }
    void SetConfigs(const std::vector<USBConfig> &configs)
    {
        this->configs = configs;
    }

    uint8_t GetDevAddr() const
    {
        return devAddr;
    }
    uint8_t GetBusNum() const
    {
        return busNum;
    }
    bool GetDescConfigCount()
    {
        return descConfigCount;
    }

    void SetDevAddr(uint8_t Num)
    {
        devAddr = Num;
    }
    void SetBusNum(uint8_t Num)
    {
        busNum = Num;
    }
    void SetName(const std::string &Name)
    {
        mName = Name;
    }
    void SetManufacturerName(const std::string &Name)
    {
        mManufacturerName = Name;
    }
    void SetProductName(const std::string &Name)
    {
        mProductName = Name;
    }
    void SetVersion(const std::string &Name)
    {
        mVersion = Name;
    }
    void SetVendorId(int Id)
    {
        mVendorId = Id;
    }
    void SetProductId(int Id)
    {
        mProductId = Id;
    }
    void SetClass(int Id)
    {
        mClass = Id;
    }
    void SetSubclass(int Id)
    {
        mSubclass = Id;
    }
    void SetProtocol(int Id)
    {
        mProtocol = Id;
    }
    void SetDescConfigCount(uint8_t count)
    {
        descConfigCount = count;
    }

    std::vector<USBConfig> &GetConfigs()
    {
        return configs;
    }

    std::string ToString() const
    {
        std::ostringstream ss;
        ss << "mName=" << mName << ","
           << "mManufacturerName=" << mManufacturerName << ","
           << "mProductName=" << mProductName << ","
           << "mVersion=" << mVersion << ","
           << "mSerial=" << mSerial << ","
           << "busNum=" << (int32_t)busNum << ","
           << "devAddr=" << (int32_t)devAddr << ","
           << "mVendorId=" << mVendorId << ","
           << "mProductId=" << mProductId << ","
           << "mClass=" << mClass << ","
           << "mSubclass=" << mSubclass << ","
           << "mProtocol=" << mProtocol << "";
        std::string str = "UsbDevice[" + ss.str() + "];    ";
        ss.str("");
        std::string strConfigs;
        for (size_t i = 0; i < configs.size(); ++i) {
            const USBConfig &config = configs[i];
            strConfigs += config.ToString();
        }
        str += strConfigs;
        return str;
    }

    void SetiManufacturer(uint8_t idx)
    {
        this->iManufacturer = idx;
    }
    uint8_t GetiManufacturer()
    {
        return this->iManufacturer;
    }
    void SetiProduct(uint8_t idx)
    {
        this->iProduct = idx;
    }
    uint8_t GetiProduct()
    {
        return this->iProduct;
    }
    void SetiSerialNumber(uint8_t idx)
    {
        this->iSerialNumber = idx;
    }
    uint8_t GetiSerialNumber()
    {
        return this->iSerialNumber;
    }
    void SetmSerial(std::string Name)
    {
        this->mSerial = Name;
    }
    const std::string GetmSerial() const
    {
        return this->mSerial;
    }
    void SetbMaxPacketSize0(uint8_t val)
    {
        this->bMaxPacketSize0 = val;
    }
    uint8_t GetbMaxPacketSize0()
    {
        return this->bMaxPacketSize0;
    }
    void SetbcdUSB(uint16_t val)
    {
        this->bcdUSB = val;
    }
    uint16_t GetbcdUSB()
    {
        return this->bcdUSB;
    }
    void SetbcdDevice(uint16_t val)
    {
        this->bcdDevice = val;
    }
    uint16_t GetbcdDevice()
    {
        return this->bcdDevice;
    }

private:
    std::string mName;
    std::string mManufacturerName;
    std::string mProductName;
    std::string mVersion;
    std::string mSerial;
    uint8_t devAddr;
    uint8_t busNum;
    uint8_t descConfigCount = UINT8_MAX;

    uint8_t bMaxPacketSize0 = UINT8_MAX;
    uint16_t bcdUSB = UINT16_MAX;
    uint16_t bcdDevice = UINT16_MAX;
    uint8_t iManufacturer = UINT8_MAX;
    uint8_t iProduct = UINT8_MAX;
    uint8_t iSerialNumber = UINT8_MAX;

    int mVendorId;
    int mProductId;
    int mClass;
    int mSubclass;
    int mProtocol;
    std::vector<USBConfig> configs;
};
} // namespace USB
} // namespace OHOS

#endif // USB_DEVICE_H
