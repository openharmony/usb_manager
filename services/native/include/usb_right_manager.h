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

#ifndef USB_RIGHT_MANAGER_H
#define USB_RIGHT_MANAGER_H

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "usb_common.h"

namespace OHOS {
namespace USB {
class UsbRightManager {
public:
    typedef std::vector<std::string> BundleNameList;
    typedef std::map<std::string, BundleNameList> RightMap;
    RightMap rightMap;

    void Init();
    int32_t HasRight(std::string deviceName, std::string bundleName);
    int32_t RequestRight(std::string deviceName, std::string bundleName);
    bool AddDeviceRight(std::string deviceName, std::string bundleName);
    bool RemoveDeviceRight(std::string deviceName);
};
} // namespace USB
} // namespace OHOS

#endif
