/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef USBMGR_USB_SERVER_PROXY_H
#define USBMGR_USB_SERVER_PROXY_H

#include <map>
#include <string>
#include "iremote_proxy.h"
#include "iusb_srv.h"
#include "nocopyable.h"
#include "usb_device.h"

namespace OHOS {
namespace USB {
class UsbServerProxy : public IRemoteProxy<IUsbSrv> {
public:
    explicit UsbServerProxy(const sptr<IRemoteObject> &impl) : IRemoteProxy<IUsbSrv>(impl) {}
    ~UsbServerProxy() = default;
    DISALLOW_COPY_AND_MOVE(UsbServerProxy);

    int32_t GetDevices(std::vector<UsbDevice> &deviceList) override;
    int32_t OpenDevice(uint8_t busNum, uint8_t devAddr) override;
    int32_t HasRight(std::string deviceName) override;
    int32_t RequestRight(std::string deviceName) override;
    int32_t RemoveRight(std::string deviceName) override;
    int32_t GetCurrentFunctions(int32_t &funcs) override;
    int32_t SetCurrentFunctions(int32_t funcs) override;
    int32_t UsbFunctionsFromString(std::string_view funcs) override;
    std::string UsbFunctionsToString(int32_t funcs) override;
    int32_t GetPorts(std::vector<UsbPort> &ports) override;
    int32_t GetSupportedModes(int32_t portId, int32_t &supportedModes) override;
    int32_t SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole) override;

    int32_t ClaimInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t force) override;
    int32_t ReleaseInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid) override;
    int32_t BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &bufferData,
        int32_t timeOut) override;
    int32_t BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &bufferData,
        int32_t timeOut) override;

    int32_t ControlTransfer(const UsbDev &dev, const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &bufferData) override;
    int32_t SetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t configIndex) override;
    int32_t GetActiveConfig(uint8_t busNum, uint8_t devAddr, uint8_t &configIndex) override;
    int32_t SetInterface(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t altIndex) override;
    int32_t GetRawDescriptor(uint8_t busNum, uint8_t devAddr, std::vector<uint8_t> &bufferData) override;
    int32_t GetFileDescriptor(uint8_t busNum, uint8_t devAddr, int32_t &fd) override;
    int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, const std::vector<uint8_t> &clientData,
        const std::vector<uint8_t> &bufferData) override;
    int32_t RequestWait(const UsbDev &dev, int32_t timeOut, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &bufferData) override;
    int32_t RequestCancel(uint8_t busNum, uint8_t devAddr, uint8_t interfaceid, uint8_t endpointId) override;
    int32_t Close(uint8_t busNum, uint8_t devAddr) override;

    int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IRemoteObject> &cb) override;
    int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe) override;
    int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, sptr<Ashmem> &ashmem) override;
    int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, sptr<Ashmem> &ashmem) override;
    int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe) override;

private:
    static inline BrokerDelegator<UsbServerProxy> delegator_;
    int32_t ParseUsbPort(MessageParcel &reply, std::vector<UsbPort> &result);
    int32_t SetDeviceMessage(MessageParcel &data, uint8_t busNum, uint8_t devAddr);
    int32_t SetBufferMessage(MessageParcel &data, const std::vector<uint8_t> &bufferData);
    int32_t GetBufferMessage(MessageParcel &data, std::vector<uint8_t> &bufferData);
    int32_t GetDeviceListMessageParcel(MessageParcel &data, std::vector<UsbDevice> &deviceList);
    int32_t GetDeviceMessageParcel(MessageParcel &data, UsbDevice &devInfo);
    int32_t GetDeviceConfigsMessageParcel(MessageParcel &data, std::vector<USBConfig> &configs);
    int32_t GetDeviceInterfacesMessageParcel(MessageParcel &data, std::vector<UsbInterface> &interfaces);
    int32_t GetDeviceEndpointsMessageParcel(MessageParcel &data, std::vector<USBEndpoint> &eps);
};
} // namespace USB
} // namespace OHOS

#endif // USBMGR_USB_SERVER_PROXY_H
