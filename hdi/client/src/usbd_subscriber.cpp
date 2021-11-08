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

#include "usbd_subscriber.h"
#include "usb_common.h"

namespace OHOS {
namespace USB {
int UsbdSubscriber::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t ret;
    UsbInfo info;
    PortInfo pinfo;
    USB_HILOGW(MODULE_USBD, "%{public}s:%{public}d CMD_NOTIFY_SUBSCRIBER_DEVICE_EVENT entry code:%{public}d", __func__,
               __LINE__, code);
    switch (code) {
        case CMD_NOTIFY_SUBSCRIBER_DEVICE_EVENT:
            USB_HILOGW(MODULE_USBD, "%{public}s CMD_NOTIFY_SUBSCRIBER_DEVICE_EVENT entry", __func__);
            ParserUsbInfo(data, reply, option, info);
            USB_HILOGD(MODULE_USBD,
                       "%{public}s CMD_NOTIFY_SUBSCRIBER_DEVICE_EVENT Read devInfo "
                       "status=%{public}d,busNum=%{public}d,devAddr=%{public}d",
                       __func__, info.getDevInfoStatus(), info.getDevInfoBusNum(), info.getDevInfoDevNum());
            ret = DeviceEvent(info);
            break;
        case CMD_NOTIFY_PORT_CHANGED:
            USB_HILOGW(MODULE_USBD, "%{public}s CMD_NOTIFY_PORT_CHANGED entry", __func__);
            ParserPortInfo(data, reply, option, pinfo);
            ret = PortChangedEvent(pinfo.portId, pinfo.powerRole, pinfo.dataRole, pinfo.mode);
            break;
        default:
            USB_HILOGW(MODULE_USBD, "no specific cmd for code: %{public}d", code);
            ret = ERR_INVALID_OPERATION;
            break;
    }
    return ret;
}

int32_t UsbdSubscriber::ParserUsbInfo(MessageParcel &data, MessageParcel &reply, MessageOption &option, UsbInfo &info)
{
    int32_t status;
    int32_t busNum;
    int32_t devNum;

    if (!data.ReadInt32(status)) {
        USB_HILOGW(MODULE_USBD, "Read devInfo status error");
    }
    if (!data.ReadInt32(busNum)) {
        USB_HILOGW(MODULE_USBD, "Read devInfo busNum error");
    }
    if (!data.ReadInt32(devNum)) {
        USB_HILOGW(MODULE_USBD, "Read devInfo devNum error");
    }
    info.setDevInfoStatus(status);
    info.setDevInfoBusNum(busNum);
    info.setDevInfoDevNum(devNum);

    USB_HILOGD(MODULE_USBD, "%{public}s Read devInfo status=%{public}d,busNum=%{public}d,devAddr=%{public}d", __func__,
               status, busNum, devNum);
    return 1;
}

int32_t UsbdSubscriber::ParserPortInfo(MessageParcel &data, MessageParcel &reply, MessageOption &option, PortInfo &info)
{
    int32_t portId;
    int32_t powerRole;
    int32_t dataRole;
    int32_t mode;

    if (!data.ReadInt32(portId)) {
        USB_HILOGW(MODULE_USBD, "Read PortInfo id error");
        return 0;
    }
    if (!data.ReadInt32(powerRole)) {
        USB_HILOGW(MODULE_USBD, "Read PortInfo powerRole error");
        return 0;
    }
    if (!data.ReadInt32(dataRole)) {
        USB_HILOGW(MODULE_USBD, "Read PortInfo dataRole error");
        return 0;
    }
    if (!data.ReadInt32(mode)) {
        USB_HILOGW(MODULE_USBD, "Read PortInfo mode error");
        return 0;
    }
    info.portId = portId;
    info.powerRole = powerRole;
    info.dataRole = dataRole;
    info.mode = mode;
    return 1;
}
} // namespace USB
} // namespace OHOS
