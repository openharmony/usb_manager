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

#ifndef USBMGR_INNERKITS_IUSB_SRV_H
#define USBMGR_INNERKITS_IUSB_SRV_H

#include "iremote_broker.h"
#include "iremote_object.h"
#include "usb_device.h"
#include "usb_param.h"
#include "usb_port.h"
namespace OHOS {
namespace USB {
class IUsbSrv : public IRemoteBroker {
public:
    enum {
        USB_FUN_HAS_RIGHT = 0,
        USB_FUN_REQUEST_RIGHT,
        USB_FUN_REMOVE_RIGHT,
        USB_FUN_OPEN_DEVICE,
        USB_FUN_GET_DEVICE,
        USB_FUN_GET_DEVICES,
        USB_FUN_GET_CURRENT_FUNCTIONS,
        USB_FUN_SET_CURRENT_FUNCTIONS,
        USB_FUN_USB_FUNCTIONS_FROM_STRING,
        USB_FUN_USB_FUNCTIONS_TO_STRING,
        USB_FUN_CLAIM_INTERFACE,
        USB_FUN_RELEASE_INTERFACE,
        USB_FUN_BULK_TRANSFER_READ,
        USB_FUN_BULK_TRANSFER_WRITE,
        USB_FUN_CONTROL_TRANSFER,
        USB_FUN_SET_ACTIVE_CONFIG,
        USB_FUN_GET_ACTIVE_CONFIG,
        USB_FUN_SET_INTERFACE,
        USB_FUN_GET_PORTS,
        USB_FUN_GET_SUPPORTED_MODES,
        USB_FUN_SET_PORT_ROLE,
        USB_FUN_REQUEST_QUEUE,
        USB_FUN_REQUEST_WAIT,
        USB_FUN_REQUEST_CANCEL,
        USB_FUN_GET_DESCRIPTOR,
        USB_FUN_GET_FILEDESCRIPTOR,
        USB_FUN_CLOSE_DEVICE,
        USB_FUN_BULK_AYSNC_READ,
        USB_FUN_BULK_AYSNC_WRITE,
        USB_FUN_BULK_AYSNC_CANCEL,
        USB_FUN_REG_BULK_CALLBACK,
        USB_FUN_UNREG_BULK_CALLBACK,
    };

    virtual int32_t OpenDevice(uint8_t busNum, uint8_t devAddr) = 0;
    virtual int32_t HasRight(std::string deviceName) = 0;
    virtual int32_t RequestRight(std::string deviceName) = 0;
    virtual int32_t RemoveRight(std::string deviceName) = 0;
    virtual int32_t GetDevices(std::vector<UsbDevice> &deviceList) = 0;
    virtual int32_t GetCurrentFunctions(int32_t &funcs) = 0;
    virtual int32_t SetCurrentFunctions(int32_t funcs) = 0;
    virtual int32_t UsbFunctionsFromString(std::string funcs) = 0;
    virtual std::string UsbFunctionsToString(int32_t funcs) = 0;
    virtual int32_t GetPorts(std::vector<UsbPort> &ports) = 0;
    virtual int32_t GetSupportedModes(int32_t portId, int32_t &supportedModes) = 0;
    virtual int32_t SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole) = 0;
    virtual int32_t ReleaseInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid) = 0;
    virtual int32_t ClaimInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t force) = 0;
    virtual int32_t BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &bufferData,
                int32_t timeOut) = 0;
    virtual int32_t BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &bufferData,
                int32_t timeOut) = 0;
    virtual int32_t ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
                std::vector<uint8_t> &bufferData) = 0;
    virtual int32_t SetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t configId) = 0;
    virtual int32_t GetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t &configId) = 0;
    virtual int32_t SetInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t altIndex) = 0;
    virtual int32_t GetRawDescriptor(uint8_t busNum, uint8_t devAddr, std::vector<uint8_t> &bufferData) = 0;
    virtual int32_t GetFileDescriptor(uint8_t busNum, uint8_t devAddr, int32_t &fd) = 0;
    virtual int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &clientData,
                const std::vector<uint8_t> &bufferData) = 0;
    virtual int32_t RequestWait(const UsbDev &dev, int32_t timeOut, std::vector<uint8_t> &clientData,
                std::vector<uint8_t> &bufferData) = 0;
    virtual int32_t RequestCancel(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t endpointId) = 0;
    virtual int32_t Close(uint8_t busNum, uint8_t devAddr) = 0;

    virtual int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const sptr<IRemoteObject> &cb) = 0;
    virtual int32_t UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe) = 0;
    virtual int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem) = 0;
    virtual int32_t BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem) = 0;
    virtual int32_t BulkCancel(const UsbDev &devInfo, const UsbPipe &pipe) = 0;

public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.usb.IUsbSrv");
};
} // namespace USB
} // namespace OHOS

#endif // USBMGR_INNERKITS_IUSB_SRV_H
