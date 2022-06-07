/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef USB_SRV_SUPPORT_H

#include <string>

namespace OHOS {
namespace USB {
struct UsbSrvSupport {
    static constexpr std::string_view CONNECTED {"connected"};
    static constexpr std::string_view FUNCTION_NAME_NONE {"none"};
    static constexpr std::string_view FUNCTION_NAME_HDC {"hdc"};
    static constexpr std::string_view FUNCTION_NAME_ACM {"acm"};
    static constexpr std::string_view FUNCTION_NAME_ECM {"ecm"};
    static constexpr std::string_view FUNCTION_NAME_MTP {"mtp"};
    static constexpr std::string_view FUNCTION_NAME_PTP {"ptp"};

    enum PowerRole : int32_t {
        POWER_ROLE_NONE,
        POWER_ROLE_SOURCE,
        POWER_ROLE_SINK,
    };

    enum DataRole : int32_t {
        DTA_ROLE_NONE,
        DATA_ROLE_HOST,
        DATA_ROLE_DEVICE,
    };

    enum PortMode : int32_t {
        PORT_MODE_NONE,
        PORT_MODE_DEVICE,
        PORT_MODE_HOST,
    };

    enum Function : uint32_t {
        FUNCTION_NONE = 0,
        FUNCTION_ACM = 1,
        FUNCTION_ECM = 2,
        FUNCTION_HDC = 4,
        FUNCTION_MTP = 8,
        FUNCTION_PTP = 16,
    };
};
} // namespace USB
} // namespace OHOS

#define USB_SRV_SUPPORT_H
#endif // USB_SRV_SUPPORT_H