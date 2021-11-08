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

#include "napi_util.h"
#include <codecvt>
#include <cstdio>
#include <locale>
#include <string>
#include "hilog_wrapper.h"
#include "securec.h"

namespace OHOS {
namespace USB {
const int32_t MAX_STR_LENGTH = 1024;
void NapiUtil::JsValueToString(const napi_env &env, const napi_value &value, const int bufLen, std::string &target)
{
    if (bufLen <= 0 || bufLen > MAX_STR_LENGTH) {
        USB_HILOGE(MODULE_JS_NAPI, "%{public}s string too long malloc failed", __func__);
        return;
    }

    char *buf = (char *)malloc(bufLen);
    if (buf == nullptr) {
        USB_HILOGE(MODULE_JS_NAPI, "%{public}s nullptr js object to string malloc failed", __func__);
        return;
    }
    (void)memset_s(buf, bufLen, 0, bufLen);
    size_t result = 0;
    napi_get_value_string_utf8(env, value, buf, bufLen, &result);

    target = buf;
    free(buf);
    buf = nullptr;
}

void NapiUtil::JsObjectToString(const napi_env &env,
                                const napi_value &object,
                                const char *fieldStr,
                                const int bufLen,
                                std::string &fieldRef)
{
    if (bufLen <= 0)
        return;

    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr, &hasProperty);
    if (hasProperty) {
        napi_value field;
        napi_valuetype valueType;

        napi_get_named_property(env, object, fieldStr, &field);
        napi_typeof(env, field, &valueType);
        NAPI_ASSERT_RETURN_VOID(env, valueType == napi_string, "Wrong argument type. String expected.");
        char *buf = (char *)malloc(bufLen);
        if (buf == nullptr) {
            USB_HILOGE(MODULE_JS_NAPI, "%{public}s nullptr js object to string malloc failed", __func__);
        }
        (void)memset_s(buf, bufLen, 0, bufLen);
        size_t result = 0;
        napi_get_value_string_utf8(env, field, buf, bufLen, &result);
        fieldRef = buf;
        free(buf);
        buf = nullptr;
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s js to str no property: %{public}s", __func__, fieldStr);
    }
}

bool NapiUtil::JsObjectGetProperty(const napi_env &env,
                                   const napi_value &object,
                                   const char *fieldStr,
                                   napi_value *value)
{
    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr, &hasProperty);
    if (hasProperty) {
        napi_get_named_property(env, object, fieldStr, value);
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s js object has no property: %{public}s", __func__, fieldStr);
    }
    return hasProperty;
}

void NapiUtil::JsObjectToInt(const napi_env &env, const napi_value &object, const char *fieldStr, int &fieldRef)
{
    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr, &hasProperty);
    if (hasProperty) {
        napi_value field;
        napi_valuetype valueType;

        napi_get_named_property(env, object, fieldStr, &field);
        napi_typeof(env, field, &valueType);
        NAPI_ASSERT_RETURN_VOID(env, valueType == napi_number, "Wrong argument type. Number expected.");
        napi_get_value_int32(env, field, &fieldRef);
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s js to int no property: %{public}s", __func__, fieldStr);
    }
}

bool NapiUtil::JsUint8ArrayParse(const napi_env &env,
                                 const napi_value &object,
                                 uint8_t **uint8Buffer,
                                 size_t &bufferSize,
                                 size_t &offset)
{
    bool isTypedArray = false;
    if (napi_is_typedarray(env, object, &isTypedArray) != napi_ok || !isTypedArray) {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s invalid type", __func__);
        return false;
    }

    napi_typedarray_type type;
    napi_value buffer;

    napi_status infoStatus =
        napi_get_typedarray_info(env, object, &type, &bufferSize, (void **)uint8Buffer, &buffer, &offset);
    if (infoStatus != napi_ok) {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s get typedarray info failed, status: %{public}d", __func__, infoStatus);
        return false;
    }

    if (type != napi_uint8_array) {
        USB_HILOGW(MODULE_JS_NAPI, "%{public}s not Uint8Array type: %{public}d", __func__, type);
        return false;
    }

    return true;
}

void NapiUtil::Uint8ArrayToJsValue(const napi_env &env, uint8_t *uint8Buffer, size_t bufferSize, napi_value &result)
{
    if (uint8Buffer == nullptr || bufferSize < 0) {
        napi_get_undefined(env, &result);
        return;
    }

    uint8_t *nativeArraybuffer = new uint8_t[bufferSize];
    napi_value nativeValue = nullptr;
    napi_create_arraybuffer(env, bufferSize, (void **)&nativeArraybuffer, &nativeValue);
    memcpy_s(nativeArraybuffer, bufferSize, uint8Buffer, bufferSize);
    napi_create_typedarray(env, napi_uint8_array, bufferSize, nativeValue, 0, &result);
}

void NapiUtil::SetValueUtf8String(const napi_env &env, const char *fieldStr, const char *str, napi_value &result)
{
    napi_value value;
    napi_create_string_utf8(env, str, NAPI_AUTO_LENGTH, &value);
    napi_set_named_property(env, result, fieldStr, value);
}

void NapiUtil::SetValueInt32(const napi_env &env, const char *fieldStr, const int intValue, napi_value &result)
{
    napi_value value;
    napi_create_int32(env, intValue, &value);
    napi_set_named_property(env, result, fieldStr, value);
}

void NapiUtil::SetValueBool(const napi_env &env, const char *fieldStr, const bool boolValue, napi_value &result)
{
    napi_value value;
    napi_get_boolean(env, boolValue, &value);
    napi_set_named_property(env, result, fieldStr, value);
}
} // namespace USB
} // namespace OHOS
