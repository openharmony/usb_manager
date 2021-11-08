/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <sys/time.h>
#include "common_event_data.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "delayed_sp_singleton.h"
#include "ohos/aafwk/content/want.h"
#include "string_ex.h"
#include "usb_common.h"
#include "usb_service.h"

using namespace OHOS::AAFwk;
using namespace OHOS::EventFwk;

namespace OHOS {
namespace USB {
const int32_t MSEC_TIME = 1000;

UsbServiceSubscriber::UsbServiceSubscriber() {}

int32_t UsbServiceSubscriber::PortChangedEvent(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode)
{
    auto pms = DelayedSpSingleton<UsbService>::GetInstance();
    Want want;
    want.SetAction(CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);
    pms->UpdateUsbPort(portId, powerRole, dataRole, mode);
    CommonEventData data;
    data.SetWant(want);
    CommonEventPublishInfo publishInfo;
    publishInfo.SetOrdered(true);
    bool isSuccess = CommonEventManager::PublishCommonEvent(data, publishInfo);
    if (!isSuccess) {
        USB_HILOGE(MODULE_USB_SERVICE, "failed to publish PortChangedEvent");
    }
    return isSuccess;
}

int32_t Invoking(const UsbInfo &info, Want &want)
{
    int ret = 0;
    switch (info.getDevInfoStatus()) {
        case ACT_DEVUP:
            want.SetAction(CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED);
            break;
        case ACT_DEVDOWN:
            want.SetAction(CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED);
            break;
        case ACT_UPDEVICE:
            want.SetAction(CommonEventSupport::COMMON_EVENT_USB_ACCESSORY_ATTACHED);
            break;
        case ACT_DOWNDEVICE:
            want.SetAction(CommonEventSupport::COMMON_EVENT_USB_ACCESSORY_DETACHED);
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

int32_t UsbServiceSubscriber::DeviceEvent(const UsbInfo &info)
{
    struct timeval start;
    gettimeofday(&start, NULL);
    int status = info.getDevInfoStatus();
    int32_t ret = UEC_OK;
    Want want;
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d status:%{public}d bus:%{public}d dev:%{public}d", __func__, __LINE__,
               status, info.getDevInfoBusNum(), info.getDevInfoDevNum());

    if (Invoking(info, want) == -1) {
        return ret;
    }

    CommonEventData data;
    data.SetWant(want);
    CommonEventPublishInfo publishInfo;
    publishInfo.SetOrdered(true);
    bool isSuccess = CommonEventManager::PublishCommonEvent(data, publishInfo);
    if (!isSuccess) {
        USB_HILOGE(MODULE_USB_SERVICE, "failed to publish USB_CHANGED event");
        ret = ERR_NO_INIT;
    }
    struct timeval end;
    gettimeofday(&end, NULL);
    USB_HILOGD(MODULE_USB_SERVICE, "end call subscriber usb device tached event, takes : %{public}ld ms",
               (end.tv_sec - start.tv_sec) * MSEC_TIME + (end.tv_usec - start.tv_usec) / MSEC_TIME);

    if ((ACT_UPDEVICE == status) || (ACT_DOWNDEVICE == status)) {
        return ret;
    }
    int busNum = info.getDevInfoBusNum();
    int devAddr = info.getDevInfoDevNum();
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d status:%{public}d bus:%{public}d dev:%{public}d", __func__, __LINE__,
               status, busNum, devAddr);
    auto pms = DelayedSpSingleton<UsbService>::GetInstance();
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d status:%{public}d pms:%{public}s", __func__, __LINE__, status,
               pms == nullptr ? "NULL" : "OK");
    if (status == ACT_DEVUP) {
        USB_HILOGE(MODULE_USB_SERVICE, "usb attached");
        pms->AddDevice(busNum, devAddr);
    } else {
        USB_HILOGE(MODULE_USB_SERVICE, "usb detached");
        pms->DelDevice(busNum, devAddr);
    }

    return ret;
}
} // namespace USB
} // namespace OHOS
