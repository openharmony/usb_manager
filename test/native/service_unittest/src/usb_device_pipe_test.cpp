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

#include "usb_device_pipe_test.h"

#include <sys/time.h>

#include <iostream>
#include <vector>

#include "delayed_sp_singleton.h"
#include "hilog_wrapper.h"
#include "if_system_ability_manager.h"
#include "system_ability_definition.h"
#include "usb_srv_client.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;

const int32_t SLEEP_TIME = 3;

void UsbDevicePipeTest::SetUpTestCase(void)
{
    auto &srvClient = UsbSrvClient::GetInstance();
    auto ret = srvClient.SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest:: [Device] SetPortRole=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    if (ret != 0) {
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int32_t c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Start UsbDevicePipeTest");
}

void UsbDevicePipeTest::TearDownTestCase(void)
{
    USB_HILOGI(MODULE_USB_SERVICE, "End UsbDevicePipeTest");
}

void UsbDevicePipeTest::SetUp(void) {}

void UsbDevicePipeTest::TearDown(void) {}

/**
 * @tc.name: getDevices001
 * @tc.desc: Test functions to getDevices(std::vector<UsbDevice> &deviceList);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, getDevices001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : getDevices001 : getDevices");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::getDevices001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::getDevices001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::getDevices001 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::getDevices001 %{public}d Close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : getDevices001 : getDevices");
}

/**
 * @tc.name: UsbOpenDevice001
 * @tc.desc: Test functions of OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDevice &device, USBDevicePipe &pip)
 * @tc.desc: 正向测试：代码正常运行，返回结果为0
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbOpenDevice001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbOpenDevice001: OpenDevice");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::vector<UsbDevice> deviceList;
    auto ret = UsbSrvClient.GetDevices(deviceList);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbOpenDevice001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(deviceList.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbOpenDevice001 %{public}d size=%{public}zu", __LINE__,
               deviceList.size());
    UsbDevice device = deviceList.front();
    USBDevicePipe pipe;
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbOpenDevice001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbOpenDevice001: OpenDevice");
}

/**
 * @tc.name: Usbcontrolstansfer001
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer001 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    struct UsbCtrlTransfer ctrldata = {0b10000000, 8, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer001 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer001 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer002
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer002 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 8, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer002 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer002 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer003
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer003 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 8, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer003 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer003 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer004
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer004 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer004 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer004 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer004 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    struct UsbCtrlTransfer ctrldata = {0b10000000, 6, 0x100, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer004 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer004 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer005
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer005 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer005 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer005 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer005 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 6, 0x100, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer005 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer005 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer006
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer006 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer006 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer006 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer006 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 8;
    uint8_t buffer[255] = {};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 6, 0x100, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer006 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer006 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer007
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer007, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer007 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer007 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer007 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer007 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0X0A, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer007 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer007 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer007 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer008
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer008, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer008 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer008 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer008 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer008 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0X0A, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer008 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer008 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer008 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer009
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer009, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer009 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer009 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer009 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer009 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0X0A, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer009 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer009 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer009 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer010
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer010, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer010 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer010 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer010 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer010 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    struct UsbCtrlTransfer ctrldata = {0b10000000, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer010 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer010 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer010 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer011
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer011, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer011 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer011 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer011 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer011 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer011 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer011 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer011 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer012
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer012, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer012 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer012 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer012 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer012 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000000, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer012 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer012 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer012 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer0013
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer0013, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer0013 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0013 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0013 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0013 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {0};
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0013 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0013 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer0013 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer0014
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer0014, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer0014 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0014 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0014 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0014 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {0};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0014 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0014 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer0014 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer0015
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer0015, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer0015 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0015 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0015 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0015 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 255;
    uint8_t buffer[255] = {0};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000001, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0015 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer0015 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer0015 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer016
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer016, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer016 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer016 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer016 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer016 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 16;
    uint8_t buffer[255] = {0};
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer016 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer016 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer016 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer017
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer017, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer017 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer017 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer017 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer017 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 16;
    uint8_t buffer[255] = {0};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer017 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer017 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer017 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer018
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer018, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer018 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer018 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer018 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer018 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    uint32_t len = 16;
    uint8_t buffer[255] = {0};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer018 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer018 %{public}d Close=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer018 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer019
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer019, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer019 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer019 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer019 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer019 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0X0C, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer019 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer019 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer020
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer020, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer020 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer020 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer020 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer020 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetBusNum(255);
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0X0C, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer020 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer020 : ControlTransfer");
}

/**
 * @tc.name: Usbcontrolstansfer021
 * @tc.desc: Test functions to ControlTransfer
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Usbcontrolstansfer021, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbcontrolstansfer021 : ControlTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer021 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer021 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer021 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint32_t len = 255;
    uint8_t buffer[255] = {};
    pipe.SetDevAddr(255);
    struct UsbCtrlTransfer ctrldata = {0b10000010, 0X0C, 0, 0, 500};
    std::vector<uint8_t> ctrlbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.ControlTransfer(pipe, ctrldata, ctrlbuffer);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Usbcontrolstansfer021 %{public}d ControlTransfer=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbcontrolstansfer021 : ControlTransfer");
}

/**
 * @tc.name: UsbClaimInterface001
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface001 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().front();
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface001 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface001 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface002
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface002 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().front();
    ret = UsbSrvClient.ClaimInterface(pipe, interface, false);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface002 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface002 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface003
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface003 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface003 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface003 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface004
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface004 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface004 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface004 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface004 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, false);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface004 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface004 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface004 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface005
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface005 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface005 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface005 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface005 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().front();
    pipe.SetBusNum(255);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface005 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface005 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface005 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface006
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface006 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface006 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface006 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface006 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().front();
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface006 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface006 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface006 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface007
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface007, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface007 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface007 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface007 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface007 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface007 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface007 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface007 : ClaimInterface");
}

/**
 * @tc.name: UsbClaimInterface008
 * @tc.desc: Test functions to ClaimInterface(const UsbInterface &interface, bool force);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbClaimInterface008, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbClaimInterface008 : ClaimInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface008 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface008 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface008 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface008 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbClaimInterface008 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbClaimInterface008 : ClaimInterface");
}

/**
 * @tc.name: UsbReleaseInterface001
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface001 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface001 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface001 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface001 : ReleaseInterface");
}

/**
 * @tc.name: UsbReleaseInterface002
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface002 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface002 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface002 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface002 : ReleaseInterface");
}

/**
 * @tc.name: UsbReleaseInterface003
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface003 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface003 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface003 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface003 : ReleaseInterface");
}

/**
 * @tc.name: UsbReleaseInterface004
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface004 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface004 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface004 : ReleaseInterface");
}

/**
 * @tc.name: UsbReleaseInterface005
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface005 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface005 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface005 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface005 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface005 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface005 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface005 : ReleaseInterface");
}

/**
 * @tc.name: UsbReleaseInterface006
 * @tc.desc: Test functions to ReleaseInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbReleaseInterface006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbReleaseInterface006 : ReleaseInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface006 %{public}d ret=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface006 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface006 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    UsbInterface interface = devi.front().GetConfigs().front().GetInterfaces().at(1);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.ReleaseInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface006 %{public}d ReleaseInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbReleaseInterface006 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbReleaseInterface006 : ReleaseInterface");
}

/**
 * @tc.name: UsbBulkTransfer001
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer001 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer001 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer002
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer002 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, -5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer002 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer003
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer003 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    pipe.SetBusNum(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer003 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer004
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer004 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    pipe.SetDevAddr(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d len=%{public}d", __LINE__, len);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer004 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer004 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer005
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer005 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    point.SetInterfaceId(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer005 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer005 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer006
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer006 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "bulk read";
    uint32_t len = 255;
    point.SetAddr(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer006 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer006 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer007
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer007, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer007 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "hello world Bulk transfer007";
    uint32_t len = 255;
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer007 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer007 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer008
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer008, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer008 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "hello world Bulk transfer008";
    uint32_t len = 255;
    pipe.SetDevAddr(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer008 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer008 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer009
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer009, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer009 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "hello world Bulk transfer009";
    uint32_t len = 255;
    pipe.SetBusNum(255);
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, 500);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer009 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer009 : BulkTransfer");
}

/**
 * @tc.name: UsbBulkTransfer010
 * @tc.desc: Test functions to BulkTransfer(const USBEndpoint &endpoint, uint8_t *buffer, uint32_t &length, int32_t
 * timeout);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, UsbBulkTransfer010, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbBulkTransfer010 : BulkTransfer");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    uint8_t buffer[255] = "hello world Bulk transfer010";
    uint32_t len = 255;
    std::vector<uint8_t> bulkbuffer = {buffer, buffer + len};
    ret = UsbSrvClient.BulkTransfer(pipe, point, bulkbuffer, -5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d BulkTransfer=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::UsbBulkTransfer010 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbBulkTransfer010 : BulkTransfer");
}

/**
 * @tc.name: SetConfiguration001
 * @tc.desc: Test functions to  SetConfiguration(const USBConfig &config);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetConfiguration001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetConfiguration001 : SetConfiguration");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBConfig config = device.GetConfigs().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.SetConfiguration(pipe, config);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration001 %{public}d SetConfiguration=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetConfiguration001 : SetConfiguration");
}

/**
 * @tc.name: SetConfiguration002
 * @tc.desc: Test functions to  SetConfiguration(const USBConfig &config);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetConfiguration002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetConfiguration002 : SetConfiguration");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBConfig config = device.GetConfigs().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.SetConfiguration(pipe, config);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration002 %{public}d SetConfiguration=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetConfiguration002 : SetConfiguration");
}

/**
 * @tc.name: SetConfiguration003
 * @tc.desc: Test functions to  SetConfiguration(const USBConfig &config);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetConfiguration003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetConfiguration003 : SetConfiguration");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBConfig config = device.GetConfigs().front();
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.SetConfiguration(pipe, config);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration003 %{public}d SetConfiguration=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetConfiguration003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);

    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetConfiguration003 : SetConfiguration");
}

/**
 * @tc.name: Close001
 * @tc.desc: Test functions to  Close();
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Close001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Close001 : Close");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close001 %{public}d size=%{public}zu", __LINE__, devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close001 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close001 %{public}d close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Close001 : Close");
}

/**
 * @tc.name: Close002
 * @tc.desc: Test functions to  Close();
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Close002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Close002 : Close");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close002 %{public}d size=%{public}zu", __LINE__, devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close002 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close002 %{public}d close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!ret);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close002 %{public}d close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Close002 : Close");
}

/**
 * @tc.name: Close003
 * @tc.desc: Test functions to  Close();
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, Close003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Close003 : Close");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close003 %{public}d size=%{public}zu", __LINE__, devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close003 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close003 %{public}d close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!ret);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close003 %{public}d close=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Close003 : Close");
}

/**
 * @tc.name: SetInterface001
 * @tc.desc: Test functions to SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface001 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface001 : SetInterface");
}

/**
 * @tc.name: SetInterface002
 * @tc.desc: Test functions to SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface002 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface002 : SetInterface");
}

/**
 * @tc.name: SetInterface003
 * @tc.desc: Test functions to SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface003 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(0);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface003 : SetInterface");
}

/**
 * @tc.name: SetInterface004
 * @tc.desc: Test functions to  SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface004 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().at(0).GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface004 : SetInterface");
}

/**
 * @tc.name: SetInterface005
 * @tc.desc: Test functions to  SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface005 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().at(0).GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface005 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface005 : SetInterface");
}

/**
 * @tc.name: SetInterface006
 * @tc.desc: Test functions to  SetInterface(const UsbInterface &interface);
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, SetInterface006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetInterface006 : SetInterface");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().at(0).GetInterfaces().at(1);
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.SetInterface(pipe, interface);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d SetInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::SetInterface006 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetInterface006 : SetInterface");
}

/**
 * @tc.name: GetRawDescriptors001
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetRawDescriptors001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetRawDescriptors001 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    ret = UsbSrvClient.GetRawDescriptors(pipe, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors001 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetRawDescriptors001 : ControlTransfer");
}

/**
 * @tc.name: GetRawDescriptors002
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetRawDescriptors002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetRawDescriptors002 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    USBDevicePipe pipeTmp = pipe;
    pipeTmp.SetBusNum(255);
    ret = UsbSrvClient.GetRawDescriptors(pipeTmp, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors002 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetRawDescriptors002 : ControlTransfer");
}

/**
 * @tc.name: GetRawDescriptors003
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetRawDescriptors003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetRawDescriptors003 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    USBDevicePipe pipeTmp = pipe;
    pipeTmp.SetDevAddr(255);
    ret = UsbSrvClient.GetRawDescriptors(pipeTmp, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetRawDescriptors003 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetRawDescriptors003 : ControlTransfer");
}

/**
 * @tc.name: GetFileDescriptors001
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetFileDescriptors001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetFileDescriptors001 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    ret = UsbSrvClient.GetRawDescriptors(pipe, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    int32_t fd = 0;
    ret = UsbSrvClient.GetFileDescriptor(pipe, fd);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d GetFileDescriptor=%{public}d",
               __LINE__, ret);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d fd=%{public}d",
               __LINE__, fd);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetFileDescriptors001 : ControlTransfer");
}

/**
 * @tc.name: GetFileDescriptors002
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetFileDescriptors002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetFileDescriptors002 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors002 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    ret = UsbSrvClient.GetRawDescriptors(pipe, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors002 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USBDevicePipe pipeTmp = pipe;
    pipeTmp.SetBusNum(255);
    int32_t fd = 0;
    ret = UsbSrvClient.GetFileDescriptor(pipeTmp, fd);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors002 %{public}d GetFileDescriptor=%{public}d",
               __LINE__, ret);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d fd=%{public}d",
               __LINE__, fd);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetFileDescriptors002 : ControlTransfer");
}

/**
 * @tc.name: GetFileDescriptors003
 * @tc.desc: Test functions to GetRawDescriptors
 * @tc.type: FUNC
 */
HWTEST_F(UsbDevicePipeTest, GetFileDescriptors003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetFileDescriptors003 : GetRawDescriptors");
    vector<UsbDevice> devi;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors003 %{public}d size=%{public}zu", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    std::vector<uint8_t> vData;
    ret = UsbSrvClient.GetRawDescriptors(pipe, vData);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors003 %{public}d GetRawDescriptors=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    USBDevicePipe pipeTmp = pipe;
    pipeTmp.SetDevAddr(255);
    int32_t fd = 0;
    ret = UsbSrvClient.GetFileDescriptor(pipeTmp, fd);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors003 %{public}d GetFileDescriptor=%{public}d",
               __LINE__, ret);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::GetFileDescriptors001 %{public}d fd=%{public}d",
               __LINE__, fd);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbDevicePipeTest::Close=%{public}d", ret);
    EXPECT_TRUE(ret);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetFileDescriptors003 : ControlTransfer");
}
