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

#include "usb_host_manager.h"
#include "common_event_data.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "hilog_wrapper.h"
#include "usb_serial_reader.h"

using namespace OHOS::AAFwk;
using namespace OHOS::EventFwk;

namespace OHOS {
namespace USB {
UsbHostManager::UsbHostManager(SystemAbility *systemAbility)
{
    systemAbility_ = systemAbility;
}

UsbHostManager::~UsbHostManager() {}

void UsbHostManager::GetDevices(MAP_STR_DEVICE &devices)
{
    devices = devices_;
}

bool UsbHostManager::DelDevice(uint8_t busNum, uint8_t devNum)
{
    std::string name = std::to_string(busNum) + "-" + std::to_string(devNum);
    MAP_STR_DEVICE::iterator iter = devices_.find(name);
    if (iter == devices_.end()) {
        USB_HILOGF(MODULE_SERVICE, "name:%{public}s bus:%{public}hhu dev:%{public}hhu not exist", name.c_str(), busNum,
            devNum);
        return false;
    }
    USB_HILOGI(
        MODULE_SERVICE, "device:%{public}s bus:%{public}hhu dev:%{public}hhu erase ", name.c_str(), busNum, devNum);
    UsbDevice *devOld = iter->second;
    if (devOld == nullptr) {
        USB_HILOGE(MODULE_SERVICE, "invalid device");
        return false;
    }

    auto isSuccess = PublishCommonEvent(CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED, *devOld);
    if (!isSuccess) {
        USB_HILOGW(MODULE_SERVICE, "send device attached broadcast failed");
    }

    delete devOld;
    devices_.erase(iter);
    return true;
}

bool UsbHostManager::AddDevice(UsbDevice *dev)
{
    if (dev == nullptr) {
        USB_HILOGF(MODULE_SERVICE, "device is NULL");
        return false;
    }

    auto isSuccess = PublishCommonEvent(CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED, *dev);
    if (!isSuccess) {
        USB_HILOGW(MODULE_SERVICE, "send device attached broadcast failed");
    }

    uint8_t busNum = dev->GetBusNum();
    uint8_t devNum = dev->GetDevAddr();
    std::string name = std::to_string(busNum) + "-" + std::to_string(devNum);
    MAP_STR_DEVICE::iterator iter = devices_.find(name);
    if (iter != devices_.end()) {
        USB_HILOGF(MODULE_SERVICE, "device:%{public}s bus:%{public}hhu dev:%{public}hhu already exist", name.c_str(),
            busNum, devNum);
        UsbDevice *devOld = iter->second;
        delete devOld;
        devices_.erase(iter);
    }
    USB_HILOGI(
        MODULE_SERVICE, "device:%{public}s bus:%{public}hhu dev:%{public}hhu insert", name.c_str(), busNum, devNum);
    devices_.insert(std::pair<std::string, UsbDevice *>(name, dev));
    return true;
}

bool UsbHostManager::PublishCommonEvent(const std::string &event, const UsbDevice &dev)
{
    Want want;
    want.SetAction(event);

    CommonEventData data(want);
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    auto jsonString = Json::writeString(builder, dev.ToJson());
    data.SetData(jsonString);

    CommonEventPublishInfo publishInfo;
    publishInfo.SetOrdered(true);
    USB_HILOGI(MODULE_SERVICE, "send %{public}s broadcast device:%{public}s", event.c_str(), jsonString.c_str());
    return CommonEventManager::PublishCommonEvent(data, publishInfo);
}
} // namespace USB
} // namespace OHOS
