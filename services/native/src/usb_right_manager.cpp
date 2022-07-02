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
#include <semaphore.h>
#include <unistd.h>

#include "display_manager.h"
#include "ui_service_mgr_client.h"
#include "usb_errors.h"

namespace OHOS {
namespace USB {
namespace {
constexpr int32_t UI_DIALOG_USB_WIDTH_NARROW = 400;
constexpr int32_t UI_DIALOG_USB_HEIGHT_NARROW = 240;
} // namespace
void UsbRightManager::Init() {}

bool UsbRightManager::HasRight(const std::string &deviceName, const std::string &bundleName)
{
    auto itMap = rightMap.find(deviceName);
    if (itMap == rightMap.end()) {
        USB_HILOGE(MODULE_USB_SERVICE, "hasRight deviceName false");
        return false;
    } else {
        BundleNameList bundleNameList = itMap->second;
        auto itVevtor = std::find(bundleNameList.begin(), bundleNameList.end(), bundleName);
        if (itVevtor == bundleNameList.end()) {
            USB_HILOGE(MODULE_USB_SERVICE, "hasRight bundleName false");
            return false;
        }
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Request Right Success");
    return true;
}

int32_t UsbRightManager::RequestRight(const std::string &deviceName, const std::string &bundleName)
{
    if (HasRight(deviceName, bundleName)) {
        USB_HILOGE(MODULE_USB_SERVICE, "device has Right ");
        return UEC_OK;
    }

    if (!GetUserAgreementByDiag(deviceName, bundleName)) {
        USB_HILOGW(MODULE_USB_SERVICE, "user don't agree");
        return UEC_SERVICE_PERMISSION_DENIED;
    }

    if (!AddDeviceRight(deviceName, bundleName)) {
        return UEC_SERVICE_INVALID_VALUE;
    }

    if (HasRight(deviceName, bundleName)) {
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

std::pair<int32_t, int32_t> UsbRightManager::GetDisplayPosition()
{
    auto display = Rosen::DisplayManager::GetInstance().GetDefaultDisplay();
    if (display == nullptr) {
        USB_HILOGI(MODULE_USB_SERVICE, "Dialog GetDefaultDisplay fail, try again.");
        display = Rosen::DisplayManager::GetInstance().GetDefaultDisplay();
    }

    if (display != nullptr) {
        USB_HILOGI(
            MODULE_USB_SERVICE, "Display size: %{public}d x %{public}d", display->GetWidth(), display->GetHeight());
        return {display->GetWidth(), display->GetHeight()};
    }

    USB_HILOGI(MODULE_USB_SERVICE, "Dialog get display fail, use default wide.");
    return {UI_DIALOG_USB_WIDTH_NARROW, UI_DIALOG_USB_HEIGHT_NARROW};
}

bool UsbRightManager::GetUserAgreementByDiag(const std::string &deviceName, const std::string &bundleName)
{
#ifdef USB_RIGHT_TEST
    return true;
#endif
    if (dialogId_ >= 0) {
        USB_HILOGI(MODULE_USB_SERVICE, "dialog is already showing");
        return false;
    }

    // show dialog
    auto [width, height] = GetDisplayPosition();
    std::string params = "{\"bundleName\":\"" + bundleName + "\", " + "\"deviceName\":\"" + deviceName + "\"}";
    bool isAgree = false;
    sem_t callbackSem;
    sem_init(&callbackSem, 1, 0);
    Ace::UIServiceMgrClient::GetInstance()->ShowDialog(
        "usb_right_dialog", params, OHOS::Rosen::WindowType::WINDOW_TYPE_SYSTEM_ALARM_WINDOW, 0, 0, width, height,
        [&callbackSem, &isAgree, this](int32_t id, const std::string &event, const std::string &params) {
            USB_HILOGI(
                MODULE_USB_SERVICE, "Shutdown dialog callback: %{public}s, %{public}s", event.c_str(), params.c_str());
            if (event == "EVENT_ALLOW") {
                isAgree = true;
            } else if (event == "EVENT_NOT_ALLOW") {
                isAgree = false;
            }
            Ace::UIServiceMgrClient::GetInstance()->CancelDialog(id);
            dialogId_ = -1;
            sem_post(&callbackSem);
        },
        &dialogId_);

    // wait call back
    sem_wait(&callbackSem);
    sem_destroy(&callbackSem);
    return isAgree;
}
} // namespace USB
} // namespace OHOS
