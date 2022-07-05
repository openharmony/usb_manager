/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
void NapiUtil::JsValueToString(const napi_env &env, const napi_value &value, const int32_t bufLen, std::string &target)
{
    if (bufLen <= 0 || bufLen > MAX_STR_LENGTH) {
        USB_HILOGE(MODULE_JS_NAPI, "string too long malloc failed");
        return;
    }
    // 1 represent '\0'
    int32_t actBufLen = bufLen + 1;
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(actBufLen);

    errno_t ret = memset_s(buf.get(), actBufLen, 0, actBufLen);
    NAPI_ASSERT_RETURN_VOID(env, ret == EOK, "JsValueToString memset_s failed.");

    size_t result = 0;
    napi_get_value_string_utf8(env, value, buf.get(), actBufLen, &result);
    target = buf.get();
}

void NapiUtil::JsObjectToString(const napi_env &env, const napi_value &object, std::string fieldStr,
    const int32_t bufLen, std::string &fieldRef)
{
    if (bufLen <= 0) {
        USB_HILOGE(MODULE_JS_NAPI, "invalid bufLen=%{public}d", bufLen);
        return;
    }

    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr.c_str(), &hasProperty);
    if (hasProperty) {
        napi_value field;
        napi_valuetype valueType;

        napi_get_named_property(env, object, fieldStr.c_str(), &field);
        napi_typeof(env, field, &valueType);
        NAPI_ASSERT_RETURN_VOID(env, valueType == napi_string, "Wrong argument type. String expected.");
        // 1 represent '\0'
        int32_t actBufLen = bufLen + 1;
        std::unique_ptr<char[]> buf = std::make_unique<char[]>(actBufLen);

        errno_t ret = memset_s(buf.get(), actBufLen, 0, actBufLen);
        NAPI_ASSERT_RETURN_VOID(env, ret == EOK, "JsObjectToString memset_s failed.");

        size_t result = 0;
        napi_get_value_string_utf8(env, field, buf.get(), actBufLen, &result);
        fieldRef = buf.get();
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "js to str no property: %{public}s", fieldStr.c_str());
    }
}

bool NapiUtil::JsObjectGetProperty(const napi_env &env, const napi_value &object, std::string fieldStr,
    napi_value &value)
{
    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr.c_str(), &hasProperty);
    if (hasProperty) {
        napi_get_named_property(env, object, fieldStr.c_str(), &value);
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "js object has no property: %{public}s", fieldStr.c_str());
    }
    return hasProperty;
}

void NapiUtil::JsObjectToInt(const napi_env &env, const napi_value &object, std::string fieldStr, int32_t &fieldRef)
{
    bool hasProperty = false;
    napi_has_named_property(env, object, fieldStr.c_str(), &hasProperty);
    if (hasProperty) {
        napi_value field;
        napi_valuetype valueType;

        napi_get_named_property(env, object, fieldStr.c_str(), &field);
        napi_typeof(env, field, &valueType);
        NAPI_ASSERT_RETURN_VOID(env, valueType == napi_number, "Wrong argument type. Number expected.");
        napi_get_value_int32(env, field, &fieldRef);
    } else {
        USB_HILOGW(MODULE_JS_NAPI, "js to int32_t no property: %{public}s", fieldStr.c_str());
    }
}

void NapiUtil::JsObjectToUint(const napi_env &env, const napi_value &object, const std::string& fieldStr,
    uint32_t &fieldRef)
{
    bool hasProperty = false;
    napi_status status = napi_has_named_property(env, object, fieldStr.c_str(), &hasProperty);
    if (status != napi_ok || !hasProperty) {
        USB_HILOGE(MODULE_JS_NAPI, "js to uint32_t no property: %{public}s", fieldStr.c_str());
        return;
    }

    napi_value field = nullptr;
    napi_valuetype valueType;

    status = napi_get_named_property(env, object, fieldStr.c_str(), &field);
    if (status != napi_ok) {
        USB_HILOGE(MODULE_JS_NAPI, "get property failed: %{public}s", fieldStr.c_str());
        return;
    }

    status = napi_typeof(env, field, &valueType);
    if (status != napi_ok) {
        USB_HILOGE(MODULE_JS_NAPI, "type error failed: %{public}s", fieldStr.c_str());
        return;
    }

    NAPI_ASSERT_RETURN_VOID(env, valueType == napi_number, "Wrong argument type. Number expected.");
    status = napi_get_value_uint32(env, field, &fieldRef);
    if (status != napi_ok) {
        USB_HILOGE(MODULE_JS_NAPI, "get value failed: %{public}s", fieldStr.c_str());
    }
}

bool NapiUtil::JsUint8ArrayParse(const napi_env &env, const napi_value &object, uint8_t **uint8Buffer,
    size_t &bufferSize, size_t &offset)
{
    bool isTypedArray = false;
    if (napi_is_typedarray(env, object, &isTypedArray) != napi_ok || !isTypedArray) {
        USB_HILOGW(MODULE_JS_NAPI, "invalid type");
        return false;
    }

    napi_typedarray_type type;
    napi_value buffer;

    napi_status infoStatus = napi_get_typedarray_info(env, object, &type, &bufferSize,
        reinterpret_cast<void **>(uint8Buffer), &buffer, &offset);
    if (infoStatus != napi_ok) {
        USB_HILOGW(MODULE_JS_NAPI, "get typedarray info failed, status: %{public}d", infoStatus);
        return false;
    }

    if (type != napi_uint8_array) {
        USB_HILOGW(MODULE_JS_NAPI, "not Uint8Array type: %{public}d", type);
        return false;
    }

    if (bufferSize == 0) {
        USB_HILOGW(MODULE_JS_NAPI, "bufferSize error");
        return false;
    }

    return true;
}

void NapiUtil::Uint8ArrayToJsValue(const napi_env &env, std::vector<uint8_t> &uint8Buffer, size_t bufferSize,
    napi_value &result)
{
    uint8_t *nativeArraybuffer = nullptr;
    napi_value nativeValue = nullptr;
    napi_create_arraybuffer(env, bufferSize, (void **)&nativeArraybuffer, &nativeValue);

    errno_t ret = memcpy_s(nativeArraybuffer, bufferSize, uint8Buffer.data(), bufferSize);
    if (ret != EOK) {
        USB_HILOGE(MODULE_JS_NAPI, "memcpy_s failed\n");
        return;
    }

    napi_create_typedarray(env, napi_uint8_array, bufferSize, nativeValue, 0, &result);
}

void NapiUtil::SetValueUtf8String(const napi_env &env, std::string fieldStr, std::string str, napi_value &result)
{
    napi_value value;
    napi_create_string_utf8(env, str.c_str(), NAPI_AUTO_LENGTH, &value);
    napi_set_named_property(env, result, fieldStr.c_str(), value);
}

void NapiUtil::SetValueInt32(const napi_env &env, std::string fieldStr, const int32_t intValue, napi_value &result)
{
    napi_value value;
    napi_create_int32(env, intValue, &value);
    napi_set_named_property(env, result, fieldStr.c_str(), value);
}

void NapiUtil::SetValueUint32(const napi_env &env, const std::string& fieldStr, const uint32_t uintValue,
    napi_value &result)
{
    napi_value value = nullptr;
    napi_status status = napi_create_uint32(env, uintValue, &value);
    if (status != napi_ok) {
        USB_HILOGE(MODULE_JS_NAPI, "create uint32 failed:%{public}s\n", fieldStr.c_str());
        return;
    }

    status = napi_set_named_property(env, result, fieldStr.c_str(), value);
    if (status != napi_ok) {
        USB_HILOGE(MODULE_JS_NAPI, "set property failed:%{public}s\n", fieldStr.c_str());
    }
}

void NapiUtil::SetValueBool(const napi_env &env, std::string fieldStr, const bool boolValue, napi_value &result)
{
    napi_value value;
    napi_get_boolean(env, boolValue, &value);
    napi_set_named_property(env, result, fieldStr.c_str(), value);
}
} // namespace USB
} // namespace OHOS
