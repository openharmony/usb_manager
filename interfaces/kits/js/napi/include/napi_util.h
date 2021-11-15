/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef NAPI_UTIL_H
#define NAPI_UTIL_H
#include <string>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
namespace OHOS {
namespace USB {
class NapiUtil {
public:
    static void JsValueToString(const napi_env &env, const napi_value &value, const int bufLen, std::string &target);
    static void JsObjectToString(const napi_env &env,
                                 const napi_value &object,
                                 const char *fieldStr,
                                 const int bufLen,
                                 std::string &fieldRef);
    static bool
        JsObjectGetProperty(const napi_env &env, const napi_value &object, const char *fieldStr, napi_value *value);
    static void JsObjectToInt(const napi_env &env, const napi_value &object, const char *fieldStr, int &fieldRef);
    static bool JsUint8ArrayParse(const napi_env &env,
                                  const napi_value &object,
                                  uint8_t **uint8Buffer,
                                  size_t &bufferSize,
                                  size_t &offset);
    static void Uint8ArrayToJsValue(const napi_env &env, uint8_t *uint8Buffer, size_t bufferSize, napi_value &result);
    static void SetValueUtf8String(const napi_env &env, const char *fieldStr, const char *str, napi_value &result);
    static void SetValueInt32(const napi_env &env, const char *fieldStr, const int intValue, napi_value &result);
    static void SetValueBool(const napi_env &env, const char *fieldStr, const bool boolValue, napi_value &result);
};
} // namespace USB
} // namespace OHOS
#endif // NAPI_UTIL_H
