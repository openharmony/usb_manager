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

#ifndef USBMGR_USB_SERVICE_H
#define USBMGR_USB_SERVICE_H

#include <map>
#include <vector>
#include "delayed_sp_singleton.h"
#include "iremote_object.h"
#include "iusb_srv.h"
#include "system_ability.h"
#include "usb_function_manager.h"
#include "usb_host_manager.h"
#include "usb_port_manager.h"
#include "usb_right_manager.h"
#include "usb_server_event_handler.h"
#include "usb_server_stub.h"
#include "usb_service_subscriber.h"
#include "usbd_client.h"
#include "usbd_subscriber.h"
#include "usbd_type.h"

namespace OHOS {
namespace USB {
class UsbService : public SystemAbility, public UsbServerStub {
    DECLARE_SYSTEM_ABILITY(UsbService)
    DECLARE_DELAYED_SP_SINGLETON(UsbService);

public:
    void OnStart() override;
    void OnStop() override;

    bool IsServiceReady() const
    {
        return ready_;
    }
    std::shared_ptr<UsbServerEventHandler> GetHandler() const
    {
        return handler_;
    }

    int32_t OpenDevice(uint8_t busNum, uint8_t devAddr) override;
    int32_t HasRight(std::string deviceName) override;
    int32_t RequestRight(std::string deviceName) override;
    int32_t RemoveRight(std::string deviceName) override;
    int32_t GetDevices(std::vector<UsbDevice> &deviceList) override;
    int32_t GetCurrentFunctions(int32_t &funcs) override;
    int32_t SetCurrentFunctions(int32_t funcs) override;
    int32_t UsbFunctionsFromString(std::string_view funcs) override;
    std::string UsbFunctionsToString(int32_t funcs) override;
    int32_t GetPorts(std::vector<UsbPort> &ports) override;
    int32_t GetSupportedModes(int32_t portId, int32_t &result) override;
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
    bool AddDevice(uint8_t busNum, uint8_t devAddr);
    bool DelDevice(uint8_t busNum, uint8_t devAddr);
    void UpdateUsbPort(int32_t portId, int32_t powerRole, int32_t dataRole, int32_t mode);
    int32_t GetDeviceInfo(uint8_t busNum, uint8_t devAddr, UsbDevice &dev);
    int32_t GetDeviceInfoDescriptor(const UsbDev &uDev, std::vector<uint8_t> &decriptor, UsbDevice &dev);
    int32_t GetConfigDescriptor(UsbDevice &dev, std::vector<uint8_t> &decriptor);

    int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const sptr<IRemoteObject> &cb) override;
    int32_t UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe) override;
    int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem) override;
    int32_t BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem) override;
    int32_t BulkCancel(const UsbDev &devInfo, const UsbPipe &pipe) override;

private:
    bool Init();
    bool InitUsbd();
    bool IsCommonEventServiceAbilityExist();
    bool GetBundleName(std::string &bundleName);
    bool ready_ = false;
    int32_t commEventRetryTimes_ = 0;
    std::mutex mutex_;
    std::shared_ptr<UsbHostManager> usbHostManger_;
    std::shared_ptr<UsbRightManager> usbRightManager_;
    std::shared_ptr<UsbPortManager> usbPortManager_;
    std::shared_ptr<AppExecFwk::EventRunner> eventRunner_;
    std::shared_ptr<UsbServerEventHandler> handler_;
    sptr<UsbServiceSubscriber> usbdSubscriber_;
    std::map<std::string, PortInfo> portMap;
};
} // namespace USB
} // namespace OHOS
#endif // USBMGR_USB_SERVICE_H
