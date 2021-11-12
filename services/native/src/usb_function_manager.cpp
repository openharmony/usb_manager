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

#include "usb_function_manager.h"
#include "usb_errors.h"

namespace OHOS {
namespace USB {
const std::string UsbFunctionManager::FUNCTION_NAME_NONE = "none";
const std::string UsbFunctionManager::FUNCTION_NAME_HDC = "hdc";
const std::string UsbFunctionManager::FUNCTION_NAME_ACM = "acm";
const std::string UsbFunctionManager::FUNCTION_NAME_ECM = "ecm";

const int32_t UsbFunctionManager::FUNCTION_NONE = 0;
const int32_t UsbFunctionManager::FUNCTION_ACM = 1;
const int32_t UsbFunctionManager::FUNCTION_ECM = 2;
const int32_t UsbFunctionManager::FUNCTION_HDC = 4;

int32_t UsbFunctionManager::currentFunctions = 0;

const int32_t UsbFunctionManager::FUNCTION_SETTABLE = FUNCTION_HDC | FUNCTION_ACM | FUNCTION_ECM;

const std::map<std::string, int32_t> UsbFunctionManager::FUNCTION_MAPPING_N2C = {
    {FUNCTION_NAME_NONE, FUNCTION_NONE},
    {FUNCTION_NAME_ACM, FUNCTION_ACM},
    {FUNCTION_NAME_ECM, FUNCTION_ECM},
    {FUNCTION_NAME_HDC, FUNCTION_HDC},
};

bool UsbFunctionManager::AreSettableFunctions(int32_t funcs)
{
    return funcs == FUNCTION_NONE || ((~FUNCTION_SETTABLE & funcs) == 0);
}

int32_t UsbFunctionManager::FromStringFunctions(std::string strFun)
{
    if (strFun.compare(FUNCTION_NAME_NONE) == 0) {
        return FUNCTION_NONE;
    }

    size_t len = strFun.length();
    if (len == 0) {
        return UEC_SERVICE_INVALID_VALUE;
    }

    std::vector<std::string> vModeStr;
    size_t pos = 0;
    while (pos < len) {
        int find_pos = strFun.find(",", pos);
        if (find_pos < 0) {
            vModeStr.push_back(strFun.substr(pos, len - pos));
            break;
        }
        vModeStr.push_back(strFun.substr(pos, find_pos - pos));
        pos = find_pos + 1;
    }

    int32_t ret = 0;
    for (auto &&item : vModeStr) {
        auto it = FUNCTION_MAPPING_N2C.find(item);
        if (it != FUNCTION_MAPPING_N2C.end()) {
            ret |= it->second;
        } else {
            USB_HILOGI(MODULE_USB_SERVICE, "UsbFunctionManager::FromStringFunctions Invalid argument of usb function");
            return UEC_SERVICE_INVALID_VALUE;
        }
    }

    return ret;
}

std::string UsbFunctionManager::ToStringFunctions(int32_t intFun)
{
    std::string stream;
    if (intFun <= FUNCTION_NONE || intFun > FUNCTION_SETTABLE) {
        stream = FUNCTION_NAME_NONE;
        return stream;
    }

    bool flag = false;
    if ((intFun & FUNCTION_HDC) != 0) {
        stream = FUNCTION_NAME_HDC;
        flag = true;
    }
    if ((intFun & FUNCTION_ACM) != 0) {
        if (flag) {
            stream = stream + ",";
        }
        stream = stream + FUNCTION_NAME_ACM;
        flag = true;
    }
    if ((intFun & FUNCTION_ECM) != 0) {
        if (flag) {
            stream = stream + ",";
        }
        stream = stream + FUNCTION_NAME_ECM;
        flag = true;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "UsbFunctionManager::ToStringFunctions success");
    return stream;
}

void UsbFunctionManager::updateFunctions(int32_t func)
{
    currentFunctions = func;
}

int32_t UsbFunctionManager::getCurrentFunctions()
{
    return currentFunctions;
}
} // namespace USB
} // namespace OHOS
