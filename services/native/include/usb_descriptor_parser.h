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

#ifndef USB_DESCRIPTOR_PARSER_H
#define USB_DESCRIPTOR_PARSER_H
#include "usb_device.h"
#include "usb_interface.h"
namespace OHOS {
namespace USB {
class UsbDescriptorParser {
public:
    UsbDescriptorParser();
    ~UsbDescriptorParser();

    static int32_t ParseDeviceDescriptor(const uint8_t *buffer, uint32_t length, UsbDevice &dev);
    static int32_t ParseConfigDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor, USBConfig &config);
    static int32_t ParseInterfaceDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor,
        UsbInterface &interface);
    static int32_t ParseEndpointDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor, USBEndpoint &ep);
};
} // namespace USB
} // namespace OHOS
#endif
