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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include "hilog_wrapper.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "napi_util.h"
#include "securec.h"
#include "usb_async_context.h"
#include "usb_device_pipe.h"
#include "usb_endpoint.h"
#include "usb_errors.h"
#include "usb_srv_client.h"

using namespace OHOS;
using namespace OHOS::USB;
const int32_t MSEC_TIME = 1000;
const int32_t INDEX_0 = 0;
const int32_t INDEX_1 = 1;
const int32_t INDEX_2 = 2;
const int32_t INDEX_3 = 3;
const int32_t PARAM_COUNT_1 = 1;
const int32_t PARAM_COUNT_2 = 2;
const int32_t PARAM_COUNT_3 = 3;
const int32_t PARAM_COUNT_4 = 4;
const int32_t STR_DEFAULT_SIZE = 256;
const int32_t DEFAULT_DESCRIPTION_SIZE = 32;

static void ParseUsbDevicePipe(napi_env env, napi_value &obj, USBDevicePipe &pip);

static void ProcessPromise(napi_env env, USBAsyncContext *asyncContext, napi_value result)
{
    if (asyncContext->deferred) {
        if (asyncContext->status == napi_ok) {
            napi_resolve_deferred(env, asyncContext->deferred, result);
        } else {
            napi_reject_deferred(env, asyncContext->deferred, result);
        }
    }
}

static void CreateUsbDevicePipe(napi_env env, napi_value &obj, const USBDevicePipe &pip)
{
    napi_create_object(env, &obj);
    NapiUtil::SetValueInt32(env, "busNum", pip.GetBusNum(), obj);
    NapiUtil::SetValueInt32(env, "devAddress", pip.GetDevAddr(), obj);
}

static void CtoJSUsbEndpoint(const napi_env &env, napi_value &obj, USBEndpoint &usbEndpoint)
{
    napi_create_object(env, &obj);
    NapiUtil::SetValueInt32(env, "address", usbEndpoint.GetAddress(), obj);
    NapiUtil::SetValueInt32(env, "attributes", usbEndpoint.GetAttributes(), obj);
    NapiUtil::SetValueInt32(env, "interval", usbEndpoint.GetInterval(), obj);
    NapiUtil::SetValueInt32(env, "maxPacketSize", usbEndpoint.GetMaxPacketSize(), obj);
    NapiUtil::SetValueInt32(env, "direction", usbEndpoint.GetDirection(), obj);
    NapiUtil::SetValueInt32(env, "number", usbEndpoint.GetEndpointNumber(), obj);
    NapiUtil::SetValueInt32(env, "type", usbEndpoint.GetType(), obj);
    NapiUtil::SetValueInt32(env, "interfaceId", usbEndpoint.GetInterfaceId(), obj);
}

static void CtoJSUsbInterface(const napi_env &env, napi_value &obj, UsbInterface &usbInterface)
{
    napi_create_object(env, &obj);
    NapiUtil::SetValueInt32(env, "id", usbInterface.GetId(), obj);
    NapiUtil::SetValueInt32(env, "protocol", usbInterface.GetProtocol(), obj);
    NapiUtil::SetValueInt32(env, "clazz", usbInterface.GetClass(), obj);
    NapiUtil::SetValueInt32(env, "subclass", usbInterface.GetSubClass(), obj);
    NapiUtil::SetValueInt32(env, "alternateSetting", usbInterface.GetAlternateSetting(), obj);
    NapiUtil::SetValueUtf8String(env, "name", usbInterface.GetName().c_str(), obj);
    napi_value arr;
    napi_create_array(env, &arr);
    for (int32_t i = 0; i < usbInterface.GetEndpointCount(); ++i) {
        USBEndpoint usbEndpoint;
        usbInterface.GetEndpoint(i, usbEndpoint);
        napi_value obj;
        CtoJSUsbEndpoint(env, obj, usbEndpoint);
        napi_set_element(env, arr, i, obj);
    }
    napi_set_named_property(env, obj, "endpoints", arr);
}

static void CtoJSUsbConfig(const napi_env &env, napi_value &obj, USBConfig &usbConfig)
{
    napi_create_object(env, &obj);
    NapiUtil::SetValueInt32(env, "id", usbConfig.GetId(), obj);
    NapiUtil::SetValueInt32(env, "attributes", usbConfig.GetAttributes(), obj);
    NapiUtil::SetValueBool(env, "isRemoteWakeup", usbConfig.IsRemoteWakeup(), obj);
    NapiUtil::SetValueBool(env, "isSelfPowered", usbConfig.IsSelfPowered(), obj);
    NapiUtil::SetValueInt32(env, "maxPower", usbConfig.GetMaxPower(), obj);
    NapiUtil::SetValueUtf8String(env, "name", usbConfig.GetName().c_str(), obj);
    napi_value arr;
    napi_create_array(env, &arr);
    for (int32_t i = 0; i < usbConfig.GetInterfaceCount(); ++i) {
        UsbInterface usbInterface;
        usbConfig.GetInterface(i, usbInterface);
        napi_value obj;
        CtoJSUsbInterface(env, obj, usbInterface);
        napi_set_element(env, arr, i, obj);
    }
    napi_set_named_property(env, obj, "interfaces", arr);
}

static void CtoJSUsbDevice(const napi_env &env, napi_value &obj, UsbDevice *usbDevice)
{
    napi_create_object(env, &obj);
    NapiUtil::SetValueUtf8String(env, "name", usbDevice->GetName().c_str(), obj);
    NapiUtil::SetValueUtf8String(env, "serial", usbDevice->GetmSerial().c_str(), obj);
    NapiUtil::SetValueUtf8String(env, "manufacturerName", usbDevice->GetManufacturerName().c_str(), obj);
    NapiUtil::SetValueUtf8String(env, "productName", usbDevice->GetProductName().c_str(), obj);
    NapiUtil::SetValueUtf8String(env, "version", usbDevice->GetVersion().c_str(), obj);
    NapiUtil::SetValueInt32(env, "vendorId", usbDevice->GetVendorId(), obj);
    NapiUtil::SetValueInt32(env, "productId", usbDevice->GetProductId(), obj);
    NapiUtil::SetValueInt32(env, "clazz", usbDevice->GetClass(), obj);
    NapiUtil::SetValueInt32(env, "subclass", usbDevice->GetSubclass(), obj);
    NapiUtil::SetValueInt32(env, "protocol", usbDevice->GetProtocol(), obj);
    NapiUtil::SetValueInt32(env, "devAddress", usbDevice->GetDevAddr(), obj);
    NapiUtil::SetValueInt32(env, "busNum", usbDevice->GetBusNum(), obj);
    napi_value arr;
    napi_create_array(env, &arr);
    for (int32_t i = 0; i < usbDevice->GetConfigCount(); ++i) {
        USBConfig usbConfig;
        usbDevice->GetConfig(i, usbConfig);
        napi_value obj;
        CtoJSUsbConfig(env, obj, usbConfig);
        napi_set_element(env, arr, i, obj);
    }
    napi_set_named_property(env, obj, "configs", arr);
}

static UsbSrvClient &g_usbClient = UsbSrvClient::GetInstance();

/* ============================================= Parsers ============================================= */
// js to c
static void ParseEndpointObj(napi_env env, napi_value endpointObj, USBEndpoint &ep)
{
    int32_t address = 0;
    NapiUtil::JsObjectToInt(env, endpointObj, "address", address);
    int32_t attributes = 0;
    NapiUtil::JsObjectToInt(env, endpointObj, "attributes", attributes);
    int32_t interval = 0;
    NapiUtil::JsObjectToInt(env, endpointObj, "interval", interval);
    int32_t maxPacketSize = 0;
    NapiUtil::JsObjectToInt(env, endpointObj, "maxPacketSize", maxPacketSize);
    int32_t interfaceId = 0;
    NapiUtil::JsObjectToInt(env, endpointObj, "interfaceId", interfaceId);
    ep = USBEndpoint(address, attributes, interval, maxPacketSize);
    ep.SetInterfaceId(interfaceId);
}

static bool ParseEndpointsObjs(napi_env env, napi_value interfaceObj, std::vector<USBEndpoint> &eps)
{
    // Get the array.
    bool result = false;
    napi_status status = napi_is_array(env, interfaceObj, &result);
    if (result && status == napi_ok) {
        napi_value endpointsObjs;
        bool isGetObjSuccess = NapiUtil::JsObjectGetProperty(env, interfaceObj, "endpoints", &endpointsObjs);
        if (!isGetObjSuccess) {
            return false;
        }

        uint32_t endpointCount = 0;
        status = napi_get_array_length(env, endpointsObjs, &endpointCount);

        for (uint32_t k = 0; k < endpointCount; ++k) {
            napi_value endpointObj;
            napi_status status = napi_get_element(env, endpointsObjs, k, &endpointObj);
            if (status != napi_ok) {
                return false;
            }
            USBEndpoint ep;
            ParseEndpointObj(env, endpointObj, ep);
            eps.push_back(ep);
        }
    }

    return false;
}

struct PipeControlParam {
    int32_t request;
    int32_t target;
    int32_t reqType;
    int32_t value;
    int32_t index;
    uint8_t *data;
    size_t dataLength;
};

static void ParsePipeControlParam(napi_env env, napi_value jsObj, PipeControlParam &controlParam)
{
    int32_t request = 0;
    NapiUtil::JsObjectToInt(env, jsObj, "request", request);
    int32_t target = 0;
    NapiUtil::JsObjectToInt(env, jsObj, "target", target);
    int32_t reqType = 0;
    NapiUtil::JsObjectToInt(env, jsObj, "reqType", reqType);
    int32_t value = 0;
    NapiUtil::JsObjectToInt(env, jsObj, "value", value);
    int32_t index = 0;
    NapiUtil::JsObjectToInt(env, jsObj, "index", index);

    napi_value dataValue;
    NapiUtil::JsObjectGetProperty(env, jsObj, "data", &dataValue);

    uint8_t *data = nullptr;
    size_t dataLength = 0;
    size_t offset = 0;
    NapiUtil::JsUint8ArrayParse(env, dataValue, &data, dataLength, offset);
    controlParam.request = request;
    controlParam.target = target;
    controlParam.reqType = reqType;
    controlParam.value = value;
    controlParam.index = index;
    controlParam.data = data;
    controlParam.dataLength = dataLength;
}

static void ParseInterfaceObj(napi_env env, napi_value interfaceObj, UsbInterface &interface)
{
    int32_t id = 0;
    NapiUtil::JsObjectToInt(env, interfaceObj, "id", id);
    int32_t protocol = 0;
    NapiUtil::JsObjectToInt(env, interfaceObj, "protocol", protocol);
    int32_t clzz = 0;
    NapiUtil::JsObjectToInt(env, interfaceObj, "clazz", clzz);
    int32_t subClass = 0;
    NapiUtil::JsObjectToInt(env, interfaceObj, "subclass", subClass);
    int32_t alternateSetting = 0;
    NapiUtil::JsObjectToInt(env, interfaceObj, "alternateSetting", alternateSetting);
    std::string name;
    NapiUtil::JsObjectToString(env, interfaceObj, "name", DEFAULT_DESCRIPTION_SIZE, name);
    std::vector<USBEndpoint> eps;
    ParseEndpointsObjs(env, interfaceObj, eps);
    interface = UsbInterface(id, protocol, clzz, subClass, alternateSetting, name, eps);
}

static bool ParseInterfacesObjs(napi_env env, napi_value configObj, std::vector<UsbInterface> &interfaces)
{
    bool result = false;
    napi_status status = napi_is_array(env, configObj, &result);
    if (result && status == napi_ok) {
        napi_value interfacesObjs;
        bool isGetObjSuccess = NapiUtil::JsObjectGetProperty(env, configObj, "interfaces", &interfacesObjs);
        if (!isGetObjSuccess) {
            return false;
        }
        uint32_t interfaceCount = 0;
        status = napi_get_array_length(env, interfacesObjs, &interfaceCount);
        for (uint32_t i = 0; i < interfaceCount; ++i) {
            napi_value interfaceObj;
            napi_status status = napi_get_element(env, interfacesObjs, i, &interfaceObj);
            if (status != napi_ok) {
                return false;
            }
            UsbInterface interface;
            ParseInterfaceObj(env, interfaceObj, interface);
            interfaces.push_back(interface);
        }
    }

    return false;
}

static void ParseConfigObj(napi_env env, napi_value configObj, USBConfig &config)
{
    int32_t id = 0;
    NapiUtil::JsObjectToInt(env, configObj, "id", id);
    int32_t attributes = 0;
    NapiUtil::JsObjectToInt(env, configObj, "attributes", attributes);
    int32_t maxPower = 0;
    NapiUtil::JsObjectToInt(env, configObj, "maxPower", maxPower);
    std::string name;
    NapiUtil::JsObjectToString(env, configObj, "name", DEFAULT_DESCRIPTION_SIZE, name);

    std::vector<UsbInterface> interfaces;
    ParseInterfacesObjs(env, configObj, interfaces);
    config = USBConfig(id, attributes, name, maxPower, interfaces);
}

static void ParseConfigsObjs(napi_env env, napi_value deviceObj, std::vector<USBConfig> &configs)
{
    napi_value configsObj;
    NapiUtil::JsObjectGetProperty(env, deviceObj, "configs", &configsObj);
    napi_valuetype valueType;
    napi_typeof(env, configsObj, &valueType);
    NAPI_ASSERT_RETURN_VOID(env, valueType == napi_object, "Wrong argument type. object expected.");

    uint32_t configCount = 0;
    napi_get_array_length(env, configsObj, &configCount);
    for (uint32_t i = 0; i < configCount; ++i) {
        napi_value configObj;
        napi_get_element(env, configsObj, i, &configObj);
        USBConfig config;
        ParseConfigObj(env, configObj, config);
        configs.push_back(config);
    }
}

static void ParseDeviceObj(napi_env env, napi_value deviceObj, UsbDevice &dev)
{
    std::string name;
    NapiUtil::JsObjectToString(env, deviceObj, "name", DEFAULT_DESCRIPTION_SIZE, name);
    std::string manufacturerName;
    NapiUtil::JsObjectToString(env, deviceObj, "manufacturerName", DEFAULT_DESCRIPTION_SIZE, manufacturerName);
    std::string productName;
    NapiUtil::JsObjectToString(env, deviceObj, "productName", DEFAULT_DESCRIPTION_SIZE, productName);
    std::string version;
    NapiUtil::JsObjectToString(env, deviceObj, "version", DEFAULT_DESCRIPTION_SIZE, version);
    int32_t devAddr = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "devAddress", devAddr);
    int32_t busNum = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "busNum", busNum);
    int32_t vendorId = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "vendorId", vendorId);
    int32_t productId = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "productId", productId);
    int32_t clazz = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "clazz", clazz);
    int32_t subclass = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "subclass", subclass);
    int32_t protocol = 0;
    NapiUtil::JsObjectToInt(env, deviceObj, "protocol", protocol);
    std::vector<USBConfig> configs;
    ParseConfigsObjs(env, deviceObj, configs);
    dev = UsbDevice(name, manufacturerName, productName, version, devAddr, busNum, vendorId, productId, clazz, subclass,
                    protocol, configs);
}

static void ParseUsbDevicePipe(napi_env env, napi_value &obj, USBDevicePipe &pip)
{
    napi_valuetype valueType;
    napi_typeof(env, obj, &valueType);
    NAPI_ASSERT_RETURN_VOID(env, valueType == napi_object, "Wrong argument type. object expected.");

    int32_t busNum = 0;
    NapiUtil::JsObjectToInt(env, obj, "busNum", busNum);
    pip.SetBusNum((uint8_t)busNum);
    int32_t devAddr = 0;
    NapiUtil::JsObjectToInt(env, obj, "devAddress", devAddr);
    pip.SetDevAddr((uint8_t)devAddr);
}

static long getTimeDiff(timeval startTime, timeval endTime)
{
    return (endTime.tv_sec - startTime.tv_sec) * MSEC_TIME + (endTime.tv_usec - startTime.tv_usec) / MSEC_TIME;
}

/* ============================================= Usb Core ============================================= */

static napi_value CoreGetDevices(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);

    std::vector<UsbDevice> deviceList;
    int32_t ret = g_usbClient.GetDevices(deviceList);
    napi_value result;
    if (ret != UEC_OK) {
        napi_get_undefined(env, &result);
        USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, get device faild ret : %{public}d", __func__, ret);
        return result;
    }

    napi_create_array(env, &result);
    int32_t i = 0;
    for (auto &ent1 : deviceList) {
        napi_value element;
        napi_create_object(env, &element);
        napi_value device;
        CtoJSUsbDevice(env, device, &ent1);
        napi_set_element(env, result, i, device);
        ++i;
    }
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value CoreConnectDevice(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "ConnectDevice failed to get cb info");

    napi_value deviceObj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, deviceObj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");
    UsbDevice dev;
    ParseDeviceObj(env, deviceObj, dev);

    USBDevicePipe pip;
    int32_t ret = g_usbClient.OpenDevice(dev, pip);
    napi_value pipObj = nullptr;
    if (ret == UEC_OK) {
        CreateUsbDevicePipe(env, pipObj, pip);
    } else if (ret == UEC_SERVICE_PERMISSION_DENIED || ret == UEC_INTERFACE_PERMISSION_DENIED) {
        napi_throw_error(env, nullptr, "permission denied, need call requestRight to get permission");
    } else {
        napi_get_undefined(env, &pipObj);
    }
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms, ret : %{public}d", __func__,
               getTimeDiff(start, end), ret);
    return pipObj;
}

static napi_value CoreHasRight(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value args[PARAM_COUNT_1] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "HasRight failed to get cb info");

    napi_valuetype type;
    NAPI_CALL(env, napi_typeof(env, args[INDEX_0], &type));
    NAPI_ASSERT(env, type == napi_string, "Wrong argument type. String expected.");
    std::string deviceName;
    NapiUtil::JsValueToString(env, args[INDEX_0], STR_DEFAULT_SIZE, deviceName);

    int32_t result = g_usbClient.HasRight(deviceName);
    USB_HILOGD(MODULE_JS_NAPI, "client called result %{public}d", result);

    napi_value napiValue = nullptr;
    napi_get_boolean(env, result == UEC_OK, &napiValue);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return napiValue;
}

static auto g_requestRightExecute = [](napi_env env, void *data) {
    struct timeval beginAsync;
    gettimeofday(&beginAsync, nullptr);
    USBRightAsyncContext *asyncContext = (USBRightAsyncContext *)data;
    int32_t ret = g_usbClient.RequestRight(asyncContext->deviceName);
    if (ret == UEC_OK) {
        asyncContext->status = napi_ok;
    } else {
        asyncContext->status = napi_generic_failure;
    }
    struct timeval endAsync;
    gettimeofday(&endAsync, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call CoreRequestRight async work finished, takes : %{public}ld ms",
               getTimeDiff(beginAsync, endAsync));
};

static auto g_requestRightComplete = [](napi_env env, napi_status status, void *data) {
    USBRightAsyncContext *asyncContext = (USBRightAsyncContext *)data;
    napi_value queryResult = nullptr;
    napi_get_boolean(env, asyncContext->status == napi_ok, &queryResult);

    ProcessPromise(env, asyncContext, queryResult);
    napi_delete_async_work(env, asyncContext->work);
    delete asyncContext;
};

static napi_value CoreRequestRight(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value args[PARAM_COUNT_1] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "RequestRight failed to get cb info");

    napi_valuetype type;
    NAPI_CALL(env, napi_typeof(env, args[INDEX_0], &type));
    NAPI_ASSERT(env, type == napi_string, "Wrong argument type. String expected.");
    std::string deviceName;
    NapiUtil::JsValueToString(env, args[INDEX_0], STR_DEFAULT_SIZE, deviceName);

    auto asyncContext = new USBRightAsyncContext();
    asyncContext->env = env;
    asyncContext->deviceName = deviceName;

    napi_value result = nullptr;
    napi_create_promise(env, &asyncContext->deferred, &result);

    napi_value resource = nullptr;
    napi_create_string_utf8(env, "RequestRight", NAPI_AUTO_LENGTH, &resource);

    napi_create_async_work(env, nullptr, resource, g_requestRightExecute, g_requestRightComplete, (void *)asyncContext,
                           &asyncContext->work);
    napi_queue_async_work(env, asyncContext->work);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value CoreUsbFunctionsFromString(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "UsbFunctionsFromString failed to get cb info");

    napi_valuetype type;
    napi_typeof(env, argv[INDEX_0], &type);
    NAPI_ASSERT(env, type == napi_string, "Wrong argument type. Function string expected.");

    // get value string argument of napi converted.
    std::string funcs;
    NapiUtil::JsValueToString(env, argv[INDEX_0], STR_DEFAULT_SIZE, funcs);

    int32_t numFuncs = g_usbClient.UsbFunctionsFromString(funcs);

    napi_value result;
    napi_create_int32(env, numFuncs, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value CoreUsbFunctionsToString(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "UsbFunctionsToString failed to get cb info");

    napi_valuetype type;
    napi_typeof(env, argv[INDEX_0], &type);
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Function number expected.");

    int32_t funcs;
    napi_get_value_int32(env, argv[INDEX_0], &funcs);
    std::string strFuncs = g_usbClient.UsbFunctionsToString(funcs);

    napi_value result;
    napi_create_string_utf8(env, strFuncs.c_str(), NAPI_AUTO_LENGTH, &result);

    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static auto g_setCurrentFunctionExecute = [](napi_env env, void *data) {
    struct timeval beginAsync;
    gettimeofday(&beginAsync, nullptr);
    USBFunctionAsyncContext *asyncContext = (USBFunctionAsyncContext *)data;
    int32_t ret = g_usbClient.SetCurrentFunctions(asyncContext->functions);
    if (ret == UEC_OK) {
        asyncContext->status = napi_ok;
    } else {
        asyncContext->status = napi_generic_failure;
    }
    struct timeval endAsync;
    gettimeofday(&endAsync, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call CoreSetCurrentFunctions async work finished, takes : %{public}ld ms",
               getTimeDiff(beginAsync, endAsync));
};

static auto g_setCurrentFunctionComplete = [](napi_env env, napi_status status, void *data) {
    USBFunctionAsyncContext *asyncContext = (USBFunctionAsyncContext *)data;
    napi_value queryResult = nullptr;
    napi_get_boolean(env, asyncContext->status == napi_ok, &queryResult);

    ProcessPromise(env, asyncContext, queryResult);
    napi_delete_async_work(env, asyncContext->work);
    delete asyncContext;
};

static napi_value CoreSetCurrentFunctions(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "SetCurrentFunctions failed to get cb info");

    napi_valuetype type;
    napi_typeof(env, argv[INDEX_0], &type);
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Number expected.");

    int32_t funcs = 0;
    napi_get_value_int32(env, argv[INDEX_0], &funcs);

    auto asyncContext = new USBFunctionAsyncContext();
    asyncContext->env = env;
    asyncContext->functions = funcs;
    napi_value result = nullptr;
    napi_create_promise(env, &asyncContext->deferred, &result);

    napi_value resource = nullptr;
    napi_create_string_utf8(env, "SetCurrentFunctions", NAPI_AUTO_LENGTH, &resource);

    napi_create_async_work(env, nullptr, resource, g_setCurrentFunctionExecute, g_setCurrentFunctionComplete,
                           (void *)asyncContext, &asyncContext->work);
    napi_queue_async_work(env, asyncContext->work);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value CoreGetCurrentFunctions(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    int32_t cfuncs;
    int32_t ret = g_usbClient.GetCurrentFunctions(cfuncs);
    napi_value result;
    if (ret != UEC_OK) {
        napi_get_undefined(env, &result);
        USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, get ports faild ret : %{public}d", __func__, ret);
        return result;
    }
    napi_create_int32(env, cfuncs, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value CoreGetPorts(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);

    std::vector<UsbPort> ports;

    int32_t ret = g_usbClient.GetPorts(ports);
    napi_value result;
    if (ret != UEC_OK) {
        napi_get_undefined(env, &result);
        USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, get ports faild ret : %{public}d", __func__, ret);
        return result;
    }

    napi_create_array(env, &result);
    for (uint32_t i = 0; i < ports.size(); ++i) {
        napi_value port;
        napi_create_object(env, &port);
        NapiUtil::SetValueInt32(env, "id", ports[i].id, port);
        NapiUtil::SetValueInt32(env, "supportedModes", ports[i].supportedModes, port);
        napi_value usbPortStatus;
        napi_create_object(env, &usbPortStatus);
        NapiUtil::SetValueInt32(env, "currentMode", ports[i].usbPortStatus.currentMode, usbPortStatus);
        NapiUtil::SetValueInt32(env, "currentPowerRole", ports[i].usbPortStatus.currentPowerRole, usbPortStatus);
        NapiUtil::SetValueInt32(env, "currentDataRole", ports[i].usbPortStatus.currentDataRole, usbPortStatus);
        napi_set_named_property(env, port, "status", usbPortStatus);
        napi_set_element(env, result, i, port);
    }
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

/* ============================================= Usb Port ============================================= */

static napi_value PortGetSupportedModes(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value args[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "GetSupportedModes failed to get cb info");

    napi_valuetype type;
    NAPI_CALL(env, napi_typeof(env, args[INDEX_0], &type));
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Number expected.");

    int32_t id = 0;
    int32_t result = 0;
    napi_get_value_int32(env, args[INDEX_0], &id);
    int32_t ret = g_usbClient.GetSupportedModes(id, result);
    if (ret) {
        USB_HILOGD(MODULE_JS_NAPI, "%{public}s:false ret = %{public}d", __func__, ret);
    }
    napi_value napiValue = nullptr;
    NAPI_CALL(env, napi_create_int32(env, result, &napiValue));
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, result : %{public}d, takes : %{public}ld ms", __func__, result,
               getTimeDiff(start, end));
    return napiValue;
}

static auto g_setPortRoleExecute = [](napi_env env, void *data) {
    struct timeval beginAsync;
    gettimeofday(&beginAsync, nullptr);
    USBPortRoleAsyncContext *asyncContext = (USBPortRoleAsyncContext *)data;
    int32_t ret = g_usbClient.SetPortRole(asyncContext->portId, asyncContext->powerRole, asyncContext->dataRole);
    if (ret == UEC_OK) {
        asyncContext->status = napi_ok;
    } else {
        asyncContext->status = napi_generic_failure;
    }
    struct timeval endAsync;
    gettimeofday(&endAsync, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call PortSetPortRole async work finished, takes : %{public}ld ms",
               getTimeDiff(beginAsync, endAsync));
};

static auto g_setPortRoleComplete = [](napi_env env, napi_status status, void *data) {
    USBPortRoleAsyncContext *asyncContext = (USBPortRoleAsyncContext *)data;
    napi_value queryResult = nullptr;

    napi_get_boolean(env, asyncContext->status == napi_ok, &queryResult);

    ProcessPromise(env, asyncContext, queryResult);
    napi_delete_async_work(env, asyncContext->work);
    delete asyncContext;
};

static napi_value PortSetPortRole(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_3;
    napi_value args[PARAM_COUNT_3] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "SetPortRole failed to get cb info");

    napi_valuetype type;
    NAPI_CALL(env, napi_typeof(env, args[INDEX_0], &type));
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Number expected.");
    NAPI_CALL(env, napi_typeof(env, args[INDEX_1], &type));
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Number expected.");
    NAPI_CALL(env, napi_typeof(env, args[INDEX_2], &type));
    NAPI_ASSERT(env, type == napi_number, "Wrong argument type. Number expected.");

    int32_t id = 0;
    napi_get_value_int32(env, args[INDEX_0], &id);
    int32_t powerRole = 0;
    napi_get_value_int32(env, args[INDEX_1], &powerRole);
    int32_t dataRole = 0;
    napi_get_value_int32(env, args[INDEX_2], &dataRole);

    auto asyncContext = new USBPortRoleAsyncContext();
    asyncContext->env = env;
    asyncContext->portId = id;
    asyncContext->dataRole = dataRole;
    asyncContext->powerRole = powerRole;

    napi_value result = nullptr;
    napi_create_promise(env, &asyncContext->deferred, &result);

    napi_value resource = nullptr;
    napi_create_string_utf8(env, "PortSetPortRole", NAPI_AUTO_LENGTH, &resource);

    napi_create_async_work(env, nullptr, resource, g_setPortRoleExecute, g_setPortRoleComplete, (void *)asyncContext,
                           &asyncContext->work);
    napi_queue_async_work(env, asyncContext->work);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value PipeClaimInterface(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_3;
    napi_value argv[PARAM_COUNT_3] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_2), "ClaimInterface failed to get cb info");

    napi_value obj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, obj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    USBDevicePipe pip;
    ParseUsbDevicePipe(env, obj, pip);

    UsbInterface interface;
    napi_value obj2 = argv[INDEX_1];
    napi_typeof(env, obj2, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");
    ParseInterfaceObj(env, obj2, interface);

    bool isForce = false;
    if (argc >= PARAM_COUNT_3) {
        napi_typeof(env, argv[INDEX_2], &type);
        NAPI_ASSERT(env, type == napi_boolean, "Wrong argument type. Object expected.");
        napi_get_value_bool(env, argv[INDEX_2], &isForce);
    }

    int32_t ret = pip.ClaimInterface(interface, isForce);
    USB_HILOGD(MODULE_JS_NAPI, "pip call ClaimInterface ret: %{public}d", ret);
    napi_value result;
    napi_create_int32(env, ret, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, ret: %{public}d takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value PipeReleaseInterface(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_2;
    napi_value argv[PARAM_COUNT_2] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_2), "ReleaseInterface failed to get cb info");

    napi_value obj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, obj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    USBDevicePipe pip;
    ParseUsbDevicePipe(env, obj, pip);

    UsbInterface interface;
    napi_value obj2 = argv[INDEX_1];
    ParseInterfaceObj(env, obj2, interface);
    int32_t ret = pip.ReleaseInterface(interface);
    USB_HILOGD(MODULE_JS_NAPI, "pip call PipeReleaseInterface ret: %{public}d", ret);
    napi_value result;
    napi_create_int32(env, ret, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, ret: %{public}d takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value PipeSetInterface(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_2;
    napi_value argv[PARAM_COUNT_2] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_2), "SetInterface failed to get cb info");

    napi_value pipeObj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, pipeObj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    USBDevicePipe pip;
    ParseUsbDevicePipe(env, pipeObj, pip);

    napi_value interfaceObj = argv[INDEX_1];
    napi_typeof(env, interfaceObj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    UsbInterface interface;
    ParseInterfaceObj(env, interfaceObj, interface);
    int32_t ret = g_usbClient.SetInterface(pip, interface);
    napi_value result;
    napi_create_int32(env, ret, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, ret: %{public}d takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value PipeSetConfiguration(napi_env env, napi_callback_info info)
{
    struct timeval start;
    gettimeofday(&start, nullptr);

    size_t argc = PARAM_COUNT_2;
    napi_value argv[PARAM_COUNT_2] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_2), "SetConfiguration failed to get cb info");

    napi_valuetype type;
    napi_value pipeObj = argv[INDEX_0];

    napi_typeof(env, pipeObj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");
    USBDevicePipe pip;
    ParseUsbDevicePipe(env, pipeObj, pip);

    napi_value configObj = argv[INDEX_1];
    napi_typeof(env, configObj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");
    USBConfig config;
    ParseConfigObj(env, configObj, config);

    int32_t ret = g_usbClient.SetConfiguration(pip, config);
    napi_value result;
    napi_create_int32(env, ret, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, ret: %{public}d, takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value PipeGetRawDescriptors(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "GetRawDescriptors failed to get cb info");
    napi_value obj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, obj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    USBDevicePipe pip;
    ParseUsbDevicePipe(env, obj, pip);

    napi_value result;
    std::vector<uint8_t> vdata;
    int32_t ret = g_usbClient.GetRawDescriptors(vdata);
    if (ret == UEC_OK) {
        NapiUtil::Uint8ArrayToJsValue(env, (uint8_t *)vdata.data(), vdata.size(), result);
    } else {
        napi_get_undefined(env, &result);
    }
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, ret: %{public}d takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value PipeGetFileDescriptor(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    int32_t fd = g_usbClient.GetFileDescriptor();
    napi_value result;
    NapiUtil::SetValueInt32(env, "fd", fd, result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static auto g_controlTransferExecute = [](napi_env env, void *data) {
    struct timeval beginAsync;
    gettimeofday(&beginAsync, nullptr);
    USBControlTransferAsyncContext *asyncContext = (USBControlTransferAsyncContext *)data;
    std::vector<uint8_t> vdata(asyncContext->buffer, asyncContext->buffer + asyncContext->bufferLength);
    const UsbCtrlTransfer tctrl = {asyncContext->reqType, asyncContext->request, asyncContext->value,
                                   asyncContext->index, asyncContext->timeout};
    int32_t ret = asyncContext->pipe.ControlTransfer(tctrl, vdata);
    if ((asyncContext->reqType & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_DIR_OUT) {
        delete asyncContext->buffer;
        asyncContext->buffer = nullptr;
    } else {
        vdata.size() > asyncContext->bufferLength ? asyncContext->bufferLength = asyncContext->bufferLength
                                                  : asyncContext->bufferLength = vdata.size();
        memcpy_s(asyncContext->buffer, asyncContext->bufferLength, vdata.data(), asyncContext->bufferLength);
    }
    if (ret == UEC_OK) {
        asyncContext->status = napi_ok;
    } else {
        asyncContext->status = napi_generic_failure;
    }
    struct timeval endAsync;
    gettimeofday(&endAsync, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call PipeControlTransfer async work finished, takes : %{public}ld ms",
               getTimeDiff(beginAsync, endAsync));
};

static auto g_controlTransferComplete = [](napi_env env, napi_status status, void *data) {
    USBControlTransferAsyncContext *asyncContext = (USBControlTransferAsyncContext *)data;
    napi_value queryResult = nullptr;

    if (asyncContext->status == napi_ok) {
        napi_create_int32(env, asyncContext->bufferLength, &queryResult);
    } else {
        USB_HILOGD(MODULE_JS_NAPI, "ControlTransfer failed");
        napi_create_int32(env, -1, &queryResult);
    }
    ProcessPromise(env, asyncContext, queryResult);
    napi_delete_async_work(env, asyncContext->work);
    delete asyncContext;
};

static napi_value PipeControlTransfer(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);

    size_t argc = PARAM_COUNT_3;
    napi_value argv[PARAM_COUNT_3] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_2), "ControlTransfer failed to get cb info");

    // pipe param
    napi_value pipeObj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, pipeObj, &type);
    NAPI_ASSERT(env, type == napi_object, "index 0 wrong argument type, object expected.");
    USBDevicePipe pipe;
    ParseUsbDevicePipe(env, pipeObj, pipe);

    // control params
    PipeControlParam controlParam;
    ParsePipeControlParam(env, argv[INDEX_1], controlParam);

    // timeout param
    int32_t timeout = 0;
    if (argc >= PARAM_COUNT_3) {
        napi_get_value_int32(env, argv[INDEX_2], &timeout);
    }

    auto asyncContext = new USBControlTransferAsyncContext();
    asyncContext->env = env;
    asyncContext->pipe = pipe;
    asyncContext->request = controlParam.request;
    asyncContext->target = controlParam.target;
    asyncContext->reqType = controlParam.reqType;
    asyncContext->value = controlParam.value;
    asyncContext->index = controlParam.index;

    if ((asyncContext->reqType & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_DIR_OUT) {
        uint8_t *nativeArraybuffer = new uint8_t[controlParam.dataLength];
        memcpy_s(nativeArraybuffer, controlParam.dataLength, controlParam.data, controlParam.dataLength);
        asyncContext->buffer = nativeArraybuffer;
    } else {
        asyncContext->buffer = controlParam.data;
    }

    asyncContext->bufferLength = controlParam.dataLength;
    asyncContext->timeout = timeout;
    napi_value result = nullptr;
    napi_create_promise(env, &asyncContext->deferred, &result);

    napi_value resource = nullptr;
    napi_create_string_utf8(env, "PipeControlTransfer", NAPI_AUTO_LENGTH, &resource);

    napi_create_async_work(env, nullptr, resource, g_controlTransferExecute, g_controlTransferComplete,
                           (void *)asyncContext, &asyncContext->work);
    napi_queue_async_work(env, asyncContext->work);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static auto g_bulkTransferExecute = [](napi_env env, void *data) {
    struct timeval beginAsync;
    gettimeofday(&beginAsync, nullptr);

    USBBulkTransferAsyncContext *asyncContext = (USBBulkTransferAsyncContext *)data;
    std::vector<uint8_t> vdata(asyncContext->buffer, asyncContext->buffer + asyncContext->bufferLength);
    int32_t ret = asyncContext->pipe.BulkTransfer(asyncContext->endpoint, vdata, asyncContext->timeout);

    if (asyncContext->endpoint.GetDirection() == USB_ENDPOINT_DIR_OUT) {
        delete asyncContext->buffer;
        asyncContext->buffer = nullptr;
    } else {
        vdata.size() > asyncContext->bufferLength ? asyncContext->bufferLength = asyncContext->bufferLength
                                                  : asyncContext->bufferLength = vdata.size();
        memcpy_s(asyncContext->buffer, asyncContext->bufferLength, vdata.data(), asyncContext->bufferLength);
    }

    USB_HILOGD(MODULE_JS_NAPI, "call pipe result %{public}d", ret);
    if (ret == UEC_OK) {
        asyncContext->status = napi_ok;
    } else {
        asyncContext->status = napi_generic_failure;
    }
    struct timeval endAsync;
    gettimeofday(&endAsync, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call PipeBulkTransfer async work finished, takes : %{public}ld ms",
               getTimeDiff(beginAsync, endAsync));
};

static auto g_bulkTransferComplete = [](napi_env env, napi_status status, void *data) {
    USBBulkTransferAsyncContext *asyncContext = (USBBulkTransferAsyncContext *)data;
    napi_value queryResult = nullptr;
    if (asyncContext->status == napi_ok) {
        napi_create_int32(env, asyncContext->bufferLength, &queryResult);
    } else {
        USB_HILOGD(MODULE_JS_NAPI, "BulkTransfer failed");
        napi_create_int32(env, -1, &queryResult);
    }
    ProcessPromise(env, asyncContext, queryResult);
    napi_delete_async_work(env, asyncContext->work);
    delete asyncContext;
};

static bool GetBulkTransferParams(napi_env env, napi_callback_info info, USBBulkTransferAsyncContext &asyncContext)
{
    size_t argc = PARAM_COUNT_4;
    napi_value argv[PARAM_COUNT_4] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_3), "BulkTransfer failed to get cb info");

    napi_valuetype type;
    USBDevicePipe pip;
    napi_typeof(env, argv[INDEX_0], &type);
    NAPI_ASSERT(env, type == napi_object, "BulkTransfer wrong argument type index 0, object expected");
    ParseUsbDevicePipe(env, argv[INDEX_0], pip);

    USBEndpoint ep;
    napi_typeof(env, argv[INDEX_1], &type);
    NAPI_ASSERT(env, type == napi_object, "BulkTransfer wrong argument type index 1. Object expected.");
    ParseEndpointObj(env, argv[INDEX_1], ep);

    int32_t timeout = 0;
    if (argc > PARAM_COUNT_3) {
        napi_get_value_int32(env, argv[INDEX_3], &timeout);
    }

    uint8_t *buffer = nullptr;
    size_t offset;
    size_t bufferSize;
    bool hasBuffer = NapiUtil::JsUint8ArrayParse(env, argv[INDEX_2], &buffer, bufferSize, offset);
    if (!hasBuffer) {
        USB_HILOGD(MODULE_JS_NAPI, "BulkTransfer wrong argument, buffer is null");
        return false;
    }
    asyncContext.env = env;
    asyncContext.pipe = pip;
    asyncContext.endpoint = ep;

    if (ep.GetDirection() == USB_ENDPOINT_DIR_OUT) {
        uint8_t *nativeArraybuffer = new uint8_t[bufferSize];
        memcpy_s(nativeArraybuffer, bufferSize, buffer, bufferSize);
        asyncContext.buffer = nativeArraybuffer;
    } else {
        asyncContext.buffer = buffer;
    }
    asyncContext.bufferLength = bufferSize;
    asyncContext.timeout = timeout;
    return true;
}

static napi_value PipeBulkTransfer(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);

    auto asyncContext = new USBBulkTransferAsyncContext();
    napi_value result = nullptr;
    napi_create_promise(env, &asyncContext->deferred, &result);
    if (!GetBulkTransferParams(env, info, *asyncContext)) {
        USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s %{public}d : invalid arg", __func__, __LINE__);
        asyncContext->status = napi_invalid_arg;
        napi_value queryResult = nullptr;
        napi_create_int32(env, -1, &queryResult);
        ProcessPromise(env, asyncContext, queryResult);
        delete asyncContext;
        return result;
    }

    napi_value resource = nullptr;
    napi_create_string_utf8(env, "PipeBulkTransfer", NAPI_AUTO_LENGTH, &resource);

    napi_status status = napi_create_async_work(env, nullptr, resource, g_bulkTransferExecute, g_bulkTransferComplete,
                                                (void *)asyncContext, &asyncContext->work);
    struct timeval end;
    gettimeofday(&end, nullptr);
    if (status != napi_ok) {
        USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, create async work failed, takes : %{public}ld ms", __func__,
                   getTimeDiff(start, end));
        return result;
    }
    napi_queue_async_work(env, asyncContext->work);
    gettimeofday(&end, nullptr);

    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, takes : %{public}ld ms", __func__, getTimeDiff(start, end));
    return result;
}

static napi_value PipeClose(napi_env env, napi_callback_info info)
{
    USB_HILOGD(MODULE_JS_NAPI, "begin call %{public}s", __func__);
    struct timeval start;
    gettimeofday(&start, nullptr);
    size_t argc = PARAM_COUNT_1;
    napi_value argv[PARAM_COUNT_1] = {0};

    napi_status status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
    NAPI_ASSERT(env, (status == napi_ok) && (argc >= PARAM_COUNT_1), "PipeClose failed to get cb info");

    napi_value obj = argv[INDEX_0];
    napi_valuetype type;
    napi_typeof(env, obj, &type);
    NAPI_ASSERT(env, type == napi_object, "Wrong argument type. Object expected.");

    USBDevicePipe pip;
    ParseUsbDevicePipe(env, obj, pip);
    int32_t ret = pip.Close();
    napi_value result;
    napi_create_int32(env, ret, &result);
    struct timeval end;
    gettimeofday(&end, nullptr);
    USB_HILOGD(MODULE_JS_NAPI, "end call %{public}s, result %{public}d, takes : %{public}ld ms", __func__, ret,
               getTimeDiff(start, end));
    return result;
}

static napi_value GetVersion(napi_env env, napi_callback_info info)
{
    std::string version = g_usbClient.GetVersion();
    USB_HILOGD(MODULE_JS_NAPI, "call %{public}s, version is %{public}s", __func__, version.c_str());
    napi_value result;
    napi_create_string_utf8(env, version.c_str(), NAPI_AUTO_LENGTH, &result);
    return result;
}

EXTERN_C_START
/*
 * function for module exports
 */
static napi_value UsbInit(napi_env env, napi_value exports)
{
    USB_HILOGD(MODULE_JS_NAPI, "enter");

    napi_property_descriptor desc[] = {
        /* usb core */
        DECLARE_NAPI_FUNCTION("getDevices", CoreGetDevices), DECLARE_NAPI_FUNCTION("connectDevice", CoreConnectDevice),
        DECLARE_NAPI_FUNCTION("hasRight", CoreHasRight), DECLARE_NAPI_FUNCTION("requestRight", CoreRequestRight),
        DECLARE_NAPI_FUNCTION("usbFunctionsFromString", CoreUsbFunctionsFromString),
        DECLARE_NAPI_FUNCTION("usbFunctionsToString", CoreUsbFunctionsToString),
        DECLARE_NAPI_FUNCTION("setCurrentFunctions", CoreSetCurrentFunctions),
        DECLARE_NAPI_FUNCTION("getCurrentFunctions", CoreGetCurrentFunctions),
        DECLARE_NAPI_FUNCTION("getPorts", CoreGetPorts),

        /* usb port */
        DECLARE_NAPI_FUNCTION("getSupportedModes", PortGetSupportedModes),
        DECLARE_NAPI_FUNCTION("setPortRoles", PortSetPortRole),

        /* usb device pipe */
        DECLARE_NAPI_FUNCTION("claimInterface", PipeClaimInterface),
        DECLARE_NAPI_FUNCTION("releaseInterface", PipeReleaseInterface),
        DECLARE_NAPI_FUNCTION("bulkTransfer", PipeBulkTransfer),
        DECLARE_NAPI_FUNCTION("controlTransfer", PipeControlTransfer),
        DECLARE_NAPI_FUNCTION("setInterface", PipeSetInterface),
        DECLARE_NAPI_FUNCTION("setConfiguration", PipeSetConfiguration),
        DECLARE_NAPI_FUNCTION("getRawDescriptor", PipeGetRawDescriptors),
        DECLARE_NAPI_FUNCTION("getFileDescriptor", PipeGetFileDescriptor),
        DECLARE_NAPI_FUNCTION("closePipe", PipeClose),

        /* fort test get usb service version */
        DECLARE_NAPI_FUNCTION("getVersion", GetVersion)};
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

    USB_HILOGD(MODULE_JS_NAPI, "return");

    return exports;
}
EXTERN_C_END

/*
 * Module definition
 */
static napi_module g_module = {.nm_version = 1,
                               .nm_flags = 0,
                               .nm_filename = "usb",
                               .nm_register_func = UsbInit,
                               .nm_modname = "usb",
                               .nm_priv = ((void *)0),
                               .reserved = {0}};

/*
 * Module registration
 */
extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&g_module);
}
