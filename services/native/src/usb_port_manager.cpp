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

#include "usb_port_manager.h"
#include "usb_errors.h"
#include "usbd_client.h"

namespace OHOS {
namespace USB {
const int32_t SUPPORTED_MODES = 3;

UsbPortManager::UsbPortManager()
{
    USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::Init start");
}

UsbPortManager::~UsbPortManager()
{
    USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::unInit start");
}

void UsbPortManager::Init()
{
    USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::QueryPort start");
    int32_t ret = QueryPort();
    if (ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbPortManager::QueryPort false");
    }
}

int32_t UsbPortManager::GetPorts(std::vector<UsbPort> &ports)
{
    if (portMap_.size() > 0) {
        for (auto it = portMap_.begin(); it != portMap_.end(); ++it) {
            ports.push_back(it->second);
        }
        USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::GetPorts success");
        return UEC_OK;
    } else {
        int32_t ret = QueryPort();
        if (ret == UEC_OK) {
            for (auto it = portMap_.begin(); it != portMap_.end(); ++it) {
                ports.push_back(it->second);
            }
            USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::QueryPort and GetPorts success");
            return ret;
        }
    }
    USB_HILOGE(MODULE_USB_SERVICE, "UsbPortManager::GetPorts false");
    return UEC_SERVICE_INVALID_VALUE;
}

int32_t UsbPortManager::GetSupportedModes(int32_t portId, int32_t &supportedModes)
{
    auto it = portMap_.find(portId);
    if (it != portMap_.end()) {
        supportedModes = it->second.supportedModes;
        USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::GetSupportedModes success");
        return UEC_OK;
    }
    USB_HILOGE(MODULE_USB_SERVICE, "UsbPortManager::GetSupportedModes false");
    return UEC_SERVICE_INVALID_VALUE;
}

int32_t UsbPortManager::QueryPort()
{
    USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::queryPorts run");
    int32_t portId = 0;
    int32_t powerRole = 0;
    int32_t dataRole = 0;
    int32_t mode = 0;
    int32_t ret = UsbdClient::GetInstance().QueryPort(portId, powerRole, dataRole, mode);
    USB_HILOGE(MODULE_USB_SERVICE, "portId:%{public}d powerRole:%{public}d dataRole:%{public}d mode:%{public}d ",
               portId, powerRole, dataRole, mode);
    if (ret) {
        USB_HILOGE(MODULE_USB_SERVICE, "UsbdClient::GetInstance().queryPorts false");
        return ret;
    }
    UsbPortStatus usbPortStatus;
    UsbPort usbPort;
    usbPortStatus.currentMode = mode;
    usbPortStatus.currentDataRole = dataRole;
    usbPortStatus.currentPowerRole = powerRole;
    usbPort.id = portId;
    usbPort.supportedModes = SUPPORTED_MODES;
    usbPort.usbPortStatus = usbPortStatus;
    AddPort(usbPort);
    return ret;
}

void UsbPortManager::UpdatePort(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode)
{
    USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::updatePort run");
    auto it = portMap_.find(portId);
    if (it != portMap_.end()) {
        if (it->second.id == portId) {
            it->second.usbPortStatus.currentPowerRole = powerRole;
            it->second.usbPortStatus.currentDataRole = dataRole;
            it->second.usbPortStatus.currentMode = mode;
            USB_HILOGI(MODULE_USB_SERVICE, "UsbPortManager::updatePort seccess");
            return;
        }
    }
    USB_HILOGE(MODULE_USB_SERVICE, "updatePort false");
}

void UsbPortManager::AddPort(UsbPort &port)
{
    USB_HILOGI(MODULE_USB_SERVICE, "addPort run");
    auto res = portMap_.insert(std::map<int32_t, UsbPort>::value_type(port.id, port));
    if (!res.second) {
        USB_HILOGW(MODULE_USB_SERVICE, "addPort port id duplicated");
        return;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "addPort successed");
}

void UsbPortManager::RemovePort(int32_t portId)
{
    USB_HILOGI(MODULE_USB_SERVICE, "removePort run");
    size_t num = portMap_.erase(portId);
    if (num == 0) {
        USB_HILOGW(MODULE_USB_SERVICE, "removePort false");
        return;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "removePort seccess");
}
} // namespace USB
} // namespace OHOS
