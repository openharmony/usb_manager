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

#include "usb_host_manager.h"
#include "hilog_wrapper.h"
#include "usb_serial_reader.h"

namespace OHOS {
namespace USB {
UsbHostManager::UsbHostManager(SystemAbility *systemAbility)
{
    systemAbility_ = systemAbility;
}

UsbHostManager::~UsbHostManager() {}

void UsbHostManager::getDevices(MAP_STR_DEVICE &Devices)
{
    Devices = devices_;
}

bool UsbHostManager::DelDevice(uint8_t busNum, uint8_t devNum)
{
    std::string name = std::to_string(busNum) + "-" + std::to_string(devNum);
    MAP_STR_DEVICE::iterator iter = devices_.find(name);
    if (iter == devices_.end()) {
        USB_HILOGF(MODULE_SERVICE, "%{public}s:%{public}d device:%{public}s bus:%{public}d dev:%{public}d not exist",
                   __func__, __LINE__, name.c_str(), busNum, devNum);
        return false;
    }
    USB_HILOGF(MODULE_SERVICE, "%{public}s:%{public}d device:%{public}s bus:%{public}d dev:%{public}d erase ", __func__,
               __LINE__, name.c_str(), busNum, devNum);
    UsbDevice *devOld = iter->second;
    devices_.erase(iter);
    if (devOld)
        delete devOld;
    return true;
}
bool UsbHostManager::AddDevice(UsbDevice *dev)
{
    if (dev == NULL) {
        USB_HILOGF(MODULE_SERVICE, "%{public}s:%{public}d device is NULL", __func__, __LINE__);
        return false;
    }
    uint8_t busNum = dev->GetBusNum();
    uint8_t devNum = dev->GetDevAddr();
    std::string name = std::to_string(busNum) + "-" + std::to_string(devNum);
    MAP_STR_DEVICE::iterator iter = devices_.find(name);
    if (iter != devices_.end()) {
        USB_HILOGF(MODULE_SERVICE,
                   "%{public}s:%{public}d device:%{public}s  bus:%{public}d dev:%{public}d already exist", __func__,
                   __LINE__, name.c_str(), busNum, devNum);
        UsbDevice *devOld = iter->second;
        devices_.erase(iter);
        if (devOld)
            delete devOld;
    }
    USB_HILOGF(MODULE_SERVICE, "%{public}s:%{public}d device:%{public}s  bus:%{public}d dev:%{public}d insert",
               __func__, __LINE__, name.c_str(), busNum, devNum);
    devices_.insert(std::pair<std::string, UsbDevice *>(name, dev));
    return true;
}
} // namespace USB
} // namespace OHOS
