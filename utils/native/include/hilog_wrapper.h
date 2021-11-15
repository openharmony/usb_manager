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

#ifndef HILOG_WRAPPER_H
#define HILOG_WRAPPER_H

#define CONFIG_HILOG
#ifdef CONFIG_HILOG
#include "hilog/log.h"
namespace OHOS {
namespace USB {
#define USBFILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define FORMATED_USB(fmt, ...) "[%{public}s] %{public}s# " fmt, USBFILENAME, __FUNCTION__, ##__VA_ARGS__

#ifdef USB_HILOGF
#undef USB_HILOGF
#endif

#ifdef USB_HILOGE
#undef USB_HILOGE
#endif

#ifdef USB_HILOGW
#undef USB_HILOGW
#endif

#ifdef USB_HILOGI
#undef USB_HILOGI
#endif

#ifdef USB_HILOGD
#undef USB_HILOGD
#endif

// param of log interface, such as USB_HILOGF.
enum UsbMgrSubModule {
    MODULE_INNERKIT = 0,
    MODULE_SERVICE,
    MODULE_JAVAKIT, // java kit used, define to avoid repeat used domain
    MODULE_JNI,
    MODULE_USB_INNERKIT, // below used by usb service
    MODULE_USB_SERVICE,
    MODULE_USBD,
    MODULE_COMMON,
    MODULE_JS_NAPI,
    USBMGR_MODULE_BUTT,
};

// 0xD002900: subsystem:Usb module:Usb, reserved 8 bit.
static constexpr unsigned int BASE_USB_DOMAIN_ID = 0xD002900;

enum UsbMgrDomainId {
    USBMGR_INNERKIT_DOMAIN = BASE_USB_DOMAIN_ID + MODULE_INNERKIT,
    USBMGR_SERVICE_DOMAIN,
    USBMGR_JAVAKIT_DOMAIN, // 0xD002902
    USB_INNERKIT_DOMAIN,
    USB_SERVICE_DOMAIN,
    USBD_DOMAIN,
    COMMON_DOMAIN,
    USB_JS_NAPI,
    USB_BUTT,
};

static constexpr OHOS::HiviewDFX::HiLogLabel USB_MGR_LABEL[USBMGR_MODULE_BUTT] = {
    {LOG_CORE, USBMGR_INNERKIT_DOMAIN, "UsbMgrClient"},
    {LOG_CORE, USBMGR_SERVICE_DOMAIN, "UsbMgrService"},
    {LOG_CORE, USBMGR_JAVAKIT_DOMAIN, "UsbMgrJavaService"},
    {LOG_CORE, USBMGR_INNERKIT_DOMAIN, "UsbMgrJni"},
    {LOG_CORE, USB_INNERKIT_DOMAIN, "UsbSrvClient"},
    {LOG_CORE, USB_SERVICE_DOMAIN, "UsbService"},
    {LOG_CORE, USBD_DOMAIN, "Usbd"},
    {LOG_CORE, COMMON_DOMAIN, "UsbMgrCommon"},
    {LOG_CORE, USB_JS_NAPI, "UsbMgrJSNAPI"},
};

// In order to improve performance, do not check the module range, module should less than USBMGR_MODULE_BUTT.
#define USB_HILOGF(module, ...) (void)OHOS::HiviewDFX::HiLog::Fatal(USB_MGR_LABEL[module], FORMATED_USB(__VA_ARGS__))
#define USB_HILOGE(module, ...) (void)OHOS::HiviewDFX::HiLog::Error(USB_MGR_LABEL[module], FORMATED_USB(__VA_ARGS__))
#define USB_HILOGW(module, ...) (void)OHOS::HiviewDFX::HiLog::Warn(USB_MGR_LABEL[module], FORMATED_USB(__VA_ARGS__))
#define USB_HILOGI(module, ...) (void)OHOS::HiviewDFX::HiLog::Info(USB_MGR_LABEL[module], FORMATED_USB(__VA_ARGS__))
#define USB_HILOGD(module, ...) (void)OHOS::HiviewDFX::HiLog::Debug(USB_MGR_LABEL[module], FORMATED_USB(__VA_ARGS__))
} // namespace USB
} // namespace OHOS

#else

#define USB_HILOGF(...)
#define USB_HILOGE(...)
#define USB_HILOGW(...)
#define USB_HILOGI(...)
#define USB_HILOGD(...)

#endif // CONFIG_HILOG

#endif // HILOG_WRAPPER_H
