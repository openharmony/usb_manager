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

#ifndef USB_FUNCTION_MANAGER_H
#define USB_FUNCTION_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "usb_common.h"

namespace OHOS {
namespace USB {
class UsbFunctionManager {
public:
    static bool AreSettableFunctions(int32_t funcs);

    static int32_t FromStringFunctions(std::string_view funcs);
    static std::string ToStringFunctions(int32_t func);
    static void updateFunctions(int32_t func);
    static int32_t getCurrentFunctions();

    static constexpr std::string_view FUNCTION_NAME_NONE = "none";
    static constexpr std::string_view FUNCTION_NAME_HDC = "hdc";
    static constexpr std::string_view FUNCTION_NAME_ACM = "acm";
    static constexpr std::string_view FUNCTION_NAME_ECM = "ecm";

    static constexpr int32_t FUNCTION_NONE = 0;
    static constexpr int32_t FUNCTION_ACM = 1;
    static constexpr int32_t FUNCTION_ECM = 2;
    static constexpr int32_t FUNCTION_HDC = 4;

private:
    static constexpr int32_t FUNCTION_SETTABLE = FUNCTION_HDC | FUNCTION_ACM | FUNCTION_ECM;
    static const std::map<std::string_view, int32_t> FUNCTION_MAPPING_N2C;
    static int32_t currentFunctions;
};
} // namespace USB
} // namespace OHOS

#endif // USB_FUNCTION_MANAGER_H
