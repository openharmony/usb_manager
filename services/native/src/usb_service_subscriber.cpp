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

#include "usb_service_subscriber.h"

#include "json.h"
#include "common_event_data.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "delayed_sp_singleton.h"
#include "string_ex.h"
#include "usb_common.h"
#include "usb_errors.h"
#include "usb_service.h"
#include "want.h"

using namespace OHOS::AAFwk;
using namespace OHOS::EventFwk;

namespace OHOS {
namespace USB {
UsbServiceSubscriber::UsbServiceSubscriber() {}

int32_t UsbServiceSubscriber::PortChangedEvent(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode)
{
    auto pms = DelayedSpSingleton<UsbService>::GetInstance();
    if (pms == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "failed to GetInstance");
        return UEC_SERVICE_GET_USB_SERVICE_FAILED;
    }

    Json::Value port;
    port["portId"] = portId;
    port["powerRole"] = powerRole;
    port["dataRole"] = dataRole;
    port["mode"] = mode;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    auto jsonString = Json::writeString(builder, port);

    Want want;
    want.SetAction(CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);
    pms->UpdateUsbPort(portId, powerRole, dataRole, mode);
    CommonEventData data;
    data.SetData(jsonString);
    data.SetWant(want);
    CommonEventPublishInfo publishInfo;
    publishInfo.SetOrdered(true);
    bool isSuccess = CommonEventManager::PublishCommonEvent(data, publishInfo);
    if (!isSuccess) {
        USB_HILOGE(MODULE_USB_SERVICE, "failed to publish PortChangedEvent");
    }
    return isSuccess;
}

int32_t UsbServiceSubscriber::DeviceEvent(const UsbInfo &info)
{
    int32_t status = info.getDevInfoStatus();
    auto pms = DelayedSpSingleton<UsbService>::GetInstance();
    if (pms == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "failed to GetInstance");
        return UEC_SERVICE_GET_USB_SERVICE_FAILED;
    }

    if ((ACT_UPDEVICE == status) || (ACT_DOWNDEVICE == status)) {
        pms->UpdateDeviceState(status);
        return UEC_OK;
    }

    int32_t busNum = info.getDevInfoBusNum();
    int32_t devAddr = info.getDevInfoDevNum();
    if (status == ACT_DEVUP) {
        USB_HILOGI(MODULE_USB_SERVICE, "usb attached");
        pms->AddDevice(busNum, devAddr);
    } else {
        USB_HILOGI(MODULE_USB_SERVICE, "usb detached");
        pms->DelDevice(busNum, devAddr);
    }

    return UEC_OK;
}
} // namespace USB
} // namespace OHOS
