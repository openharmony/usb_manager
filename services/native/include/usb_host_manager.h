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

#ifndef USB_HOST_MANAGER_H
#define USB_HOST_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "system_ability.h"
#include "usb_device.h"
#include "usb_right_manager.h"

namespace OHOS {
namespace USB {
typedef std::map<std::string, UsbDevice *> MAP_STR_DEVICE;

class UsbHostManager {
public:
    explicit UsbHostManager(SystemAbility *systemAbility);
    ~UsbHostManager();
    void GetDevices(MAP_STR_DEVICE &devices);
    bool DelDevice(uint8_t busNum, uint8_t devNum);
    bool AddDevice(UsbDevice *dev);

private:
    MAP_STR_DEVICE devices_;
    SystemAbility *systemAbility_;
};
} // namespace USB
} // namespace OHOS

#endif
