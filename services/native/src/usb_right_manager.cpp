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

#include "usb_right_manager.h"
#include "usb_errors.h"

namespace OHOS {
namespace USB {
void UsbRightManager::Init() {}

int32_t UsbRightManager::HasRight(const std::string &deviceName, const std::string &bundleName)
{
    auto itMap = rightMap.find(deviceName);
    if (itMap == rightMap.end()) {
        USB_HILOGE(MODULE_USB_SERVICE, "hasRight deviceName false");
        return UEC_SERVICE_INVALID_VALUE;
    } else {
        BundleNameList bundleNameList = itMap->second;
        auto itVevtor = std::find(bundleNameList.begin(), bundleNameList.end(), bundleName);
        if (itVevtor == bundleNameList.end()) {
            USB_HILOGE(MODULE_USB_SERVICE, "hasRight bundleName false");
            return UEC_SERVICE_INVALID_VALUE;
        }
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Request Right Success");
    return UEC_OK;
}

int32_t UsbRightManager::RequestRight(const std::string &deviceName, const std::string &bundleName)
{
    if (HasRight(deviceName, bundleName) == 0) {
        USB_HILOGE(MODULE_USB_SERVICE, "device has Right ");
        return UEC_OK;
    }
    if (!AddDeviceRight(deviceName, bundleName)) {
        return UEC_SERVICE_INVALID_VALUE;
    }
    if (HasRight(deviceName, bundleName) == 0) {
        USB_HILOGI(MODULE_USB_SERVICE, "requestRight Success");
        return UEC_OK;
    }
    USB_HILOGE(MODULE_USB_SERVICE, "requestRight False ");
    return UEC_SERVICE_INVALID_VALUE;
}

bool UsbRightManager::AddDeviceRight(const std::string &deviceName, const std::string &bundleName)
{
    auto itMap = rightMap.find(deviceName);
    if (itMap != rightMap.end()) {
        auto v = itMap->second;
        auto itVevtor = std::find(v.begin(), v.end(), bundleName);
        if (itVevtor != v.end()) {
            USB_HILOGE(MODULE_USB_SERVICE, "addDeviceRight false");
            return false;
        }
        itMap->second.push_back(bundleName);
        USB_HILOGI(MODULE_USB_SERVICE, "addDeviceRight success");
    }
    BundleNameList bundleNameList;
    bundleNameList.push_back(bundleName);
    rightMap.insert(RightMap::value_type(deviceName, bundleNameList));
    USB_HILOGI(MODULE_USB_SERVICE, "addDeviceRight success");
    return true;
}

bool UsbRightManager::RemoveDeviceRight(const std::string &deviceName)
{
    auto it = rightMap.find(deviceName);
    if (it != rightMap.end()) {
        rightMap.erase(it);
        USB_HILOGI(MODULE_USB_SERVICE, "removeDeviceRight success");
        return true;
    }
    return false;
}
} // namespace USB
} // namespace OHOS
