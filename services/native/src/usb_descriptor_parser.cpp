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

#include "usb_descriptor_parser.h"
#include "hilog_wrapper.h"
#include "message_parcel.h"
#include "securec.h"
#include "usb_config.h"
#include "usb_endpoint.h"
#include "usb_errors.h"
#include "usb_interface.h"
#include "usbd_type.h"

namespace OHOS {
namespace USB {
enum class DescriptorType {
    DESCRIPTOR_TYPE_DEVICE = 1,
    DESCRIPTOR_TYPE_CONFIG = 2,
    DESCRIPTOR_TYPE_INTERFACE = 4,
    DESCRIPTOR_TYPE_ENDPOINT = 5
};

UsbDescriptorParser::UsbDescriptorParser() {}

UsbDescriptorParser::~UsbDescriptorParser() {}

int32_t UsbDescriptorParser::ParseDeviceDescriptor(const uint8_t *buffer, uint32_t length, UsbDevice &dev)
{
    uint32_t cursor = 0;
    if (buffer == nullptr || length == 0) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: buffer is null", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }

    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:  parse begin buffer=%{public}p, length=%{public}d, cursor=%{public}d",
               __func__, buffer, length, cursor);
    uint32_t deviceDescriptorSize = sizeof(UsbdDeviceDescriptor);
    if (length < deviceDescriptorSize) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: buffer size error", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }

    UsbdDeviceDescriptor deviceDescriptor = *(UsbdDeviceDescriptor *)buffer;
    cursor += deviceDescriptorSize;
    if (deviceDescriptor.bLength != deviceDescriptorSize) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: UsbdDeviceDescriptor size error", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }

    dev.SetVendorId(deviceDescriptor.idVendor);
    dev.SetProductId(deviceDescriptor.idProduct);
    dev.SetClass(deviceDescriptor.bDeviceClass);
    dev.SetSubclass(deviceDescriptor.bDeviceSubClass);
    dev.SetDescConfigCount(deviceDescriptor.bNumConfigurations);

    dev.SetbMaxPacketSize0(deviceDescriptor.bMaxPacketSize0);
    dev.SetbcdDevice(deviceDescriptor.bcdDevice);
    dev.SetbcdUSB(deviceDescriptor.bcdUSB);
    dev.SetiManufacturer(deviceDescriptor.iManufacturer);
    dev.SetiProduct(deviceDescriptor.iProduct);
    dev.SetiSerialNumber(deviceDescriptor.iSerialNumber);
    return UEC_OK;
}

int32_t UsbDescriptorParser::ParseConfigDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor,
    USBConfig &config)
{
    if (buffer == nullptr || length == 0) {
        return UEC_SERVICE_INVALID_VALUE;
    }

    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:  parse begin buffer=%{public}p, length=%{public}d, cursor=%{public}d",
               __func__, buffer, length, cursor);
    uint32_t configDescriptorSize = sizeof(UsbdConfigDescriptor);
    if (length < configDescriptorSize) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:  buffer size error", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }

    UsbdConfigDescriptor configDescriptor = *(UsbdConfigDescriptor *)buffer;
    cursor += configDescriptorSize;
    if (configDescriptor.bLength != configDescriptorSize) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:  UsbdDeviceDescriptor size error", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }

    config.SetId(configDescriptor.bConfigurationValue);
    config.SetAttribute(configDescriptor.bmAttributes);
    config.SetMaxPower(configDescriptor.bMaxPower);
    config.SetiConfiguration(configDescriptor.iConfiguration);

    std::vector<UsbInterface> interfaces;
    for (int32_t i = 0; (i < configDescriptor.bNumInterfaces) && (cursor < length); ++i) {
        uint32_t interfaceCursor = 0;
        UsbInterface interface;
        ParseInterfaceDescriptor(buffer + cursor + interfaceCursor, length - cursor - interfaceCursor, interfaceCursor,
                                 interface);
        if (interface.GetEndpointCount() > 0) {
            interfaces.push_back(interface);
        } else {
            --i;
        }
        cursor += interfaceCursor;
    }
    config.SetInterfaces(interfaces);
    return UEC_OK;
}

int32_t UsbDescriptorParser::ParseInterfaceDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor,
    UsbInterface &interface)
{
    if (buffer == nullptr || length == 0) {
        return UEC_SERVICE_INVALID_VALUE;
    }

    uint32_t descriptorHeaderSize = sizeof(UsbdDescriptorHeader);
    while ((uint32_t)cursor < length) {
        if (descriptorHeaderSize >= length) {
            USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: descriptor size error", __func__);
            return UEC_SERVICE_INVALID_VALUE;
        }
        UsbdDescriptorHeader descriptorHeader = *(UsbdDescriptorHeader *)(buffer + cursor);
        if (descriptorHeader.bLength > length) {
            USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: descriptor size error", __func__);
            return UEC_SERVICE_INVALID_VALUE;
        }
        if (descriptorHeader.bDescriptorType == static_cast<uint8_t>(DescriptorType::DESCRIPTOR_TYPE_INTERFACE)) {
            break;
        }
        cursor += descriptorHeader.bLength;
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s: type = %{public}d, length=%{public}d", __func__,
                   descriptorHeader.bDescriptorType, descriptorHeader.bLength);
    }

    UsbdInterfaceDescriptor interfaceDescriptor = *(UsbdInterfaceDescriptor *)(buffer + cursor);
    if (interfaceDescriptor.bLength != sizeof(UsbdInterfaceDescriptor)) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: UsbdInterfaceDescriptor size error", __func__);
        return UEC_SERVICE_INVALID_VALUE;
    }
    cursor += interfaceDescriptor.bLength;

    interface.SetId(interfaceDescriptor.bInterfaceNumber);
    interface.SetProtocol(interfaceDescriptor.bInterfaceProtocol);
    interface.SetAlternateSetting(interfaceDescriptor.bAlternateSetting);
    interface.SetClass(interfaceDescriptor.bInterfaceClass);
    interface.SetSubClass(interfaceDescriptor.bInterfaceSubClass);
    interface.SetiInterface(interfaceDescriptor.iInterface);

    std::vector<USBEndpoint> eps;
    for (int32_t j = 0; j < interfaceDescriptor.bNumEndpoints; ++j) {
        uint32_t epCursor = 0;
        USBEndpoint ep;
        ParseEndpointDescriptor(buffer + cursor + epCursor, length - cursor - epCursor, epCursor, ep);
        ep.SetInterfaceId(interfaceDescriptor.bInterfaceNumber);
        eps.push_back(ep);
        cursor += epCursor;
    }
    interface.SetEndpoints(eps);
    return UEC_OK;
}
int32_t UsbDescriptorParser::ParseEndpointDescriptor(const uint8_t *buffer, uint32_t length, uint32_t &cursor,
    USBEndpoint &ep)
{
    USB_HILOGI(MODULE_USB_SERVICE, "%{public}s:  parse begin buffer=%{public}p, length=%{public}d, cursor=%{public}d",
               __func__, buffer, length, cursor);
    if (buffer == nullptr || length == 0) {
        return UEC_SERVICE_INVALID_VALUE;
    }

    uint32_t descriptorHeaderSize = sizeof(UsbdDescriptorHeader);
    while ((uint32_t)cursor < length) {
        if (descriptorHeaderSize >= length) {
            USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: descriptor size error", __func__);
            return UEC_SERVICE_INVALID_VALUE;
        }
        UsbdDescriptorHeader descriptorHeader = *(UsbdDescriptorHeader *)(buffer + cursor);
        if (descriptorHeader.bLength > length) {
            USB_HILOGE(MODULE_USB_SERVICE, "%{public}s: descriptor size error", __func__);
            return UEC_SERVICE_INVALID_VALUE;
        }
        if (descriptorHeader.bDescriptorType == static_cast<uint8_t>(DescriptorType::DESCRIPTOR_TYPE_ENDPOINT)) {
            break;
        }
        cursor += descriptorHeader.bLength;
        USB_HILOGI(MODULE_USB_SERVICE, "%{public}s: error type = %{public}d, length=%{public}d", __func__,
                   descriptorHeader.bDescriptorType, descriptorHeader.bLength);
    }

    UsbdEndpointDescriptor endpointDescriptor = *(UsbdEndpointDescriptor *)(buffer + cursor);
    if (endpointDescriptor.bLength != sizeof(UsbdEndpointDescriptor)) {
        USB_HILOGE(MODULE_USB_SERVICE, "%{public}s:  Endpoint descriptor size error, length=%{public}d", __func__,
                   endpointDescriptor.bLength);
        return UEC_SERVICE_INVALID_VALUE;
    }
    cursor += endpointDescriptor.bLength;

    ep.SetAddr(endpointDescriptor.bEndpointAddress);
    ep.SetAttr(endpointDescriptor.bmAttributes);
    ep.SetInterval(endpointDescriptor.bInterval);
    ep.SetMaxPacketSize(endpointDescriptor.wMaxPacketSize);
    return UEC_OK;
}
} // namespace USB
} // namespace OHOS
