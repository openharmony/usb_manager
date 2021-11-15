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

#ifndef USB_PORT_MANAGER_H
#define USB_PORT_MANAGER_H

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "usb_common.h"
#include "usb_port.h"

namespace OHOS {
namespace USB {
class UsbPortManager {
public:
    UsbPortManager();
    ~UsbPortManager();
    typedef std::map<int32_t, UsbPort> PortMap;
    PortMap portMap;

    void Init();
    void Init(int32_t test);
    int32_t GetPorts(std::vector<UsbPort> &ports);
    int32_t GetSupportedModes(int32_t portId, int32_t &supportedModes);
    void SetPortRoles(int32_t portId, int32_t powerRole, int32_t dataRole);
    int32_t QueryPort();
    void UpdatePort(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode);
    void AddPort(UsbPort &port);
    void RemovePort(int32_t portId);

private:
};
} // namespace USB
} // namespace OHOS

#endif
