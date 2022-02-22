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

#include "usb_request_test.h"
#include <csignal>
#include <iostream>
#include <iservice_registry.h>
#include <vector>
#include "if_system_ability_manager.h"
#include "ipc_skeleton.h"
#include "iusb_srv.h"
#include "system_ability_definition.h"
#include "usb_common.h"
#include "usb_device_pipe.h"
#include "usb_param.h"
#include "usb_request.h"
#include "usb_service.h"
#include "usb_srv_client.h"

const int32_t SLEEP_TIME = 3;

using namespace testing::ext;
using namespace OHOS::USB;
using namespace OHOS;
using namespace std;

void UsbRequestTest::SetUpTestCase(void)
{
    auto &srvClient = UsbSrvClient::GetInstance();
    auto ret = srvClient.SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbdRequestTest:: [Device] SetPortRole=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    if (ret != 0) {
        exit(0);
    }
    std::cout << "please connect device, press enter to continue" << std::endl;
    int32_t c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Start UsbdRequestTest");
}

void UsbRequestTest::TearDownTestCase(void)
{
    USB_HILOGI(MODULE_USB_SERVICE, "End UsbdRequestTest");
}

void UsbRequestTest::SetUp(void) {}

void UsbRequestTest::TearDown(void) {}
/**
 * @tc.name: UsbRequestInitialize001
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize001 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize001 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize001 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request read";
    std::vector<uint8_t> cdata = {tag, tag + 12};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize001 %{public}d Initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize001 : Initialize");
}

/**
 * @tc.name: UsbRequestInitialize002
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize002 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize002 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize002 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request write";
    std::vector<uint8_t> cdata = {tag, tag + 13};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> bufferdata = {buffer, buffer + 17};
    request.SetReqData(bufferdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize002 %{public}d Initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize002 : Initialize");
}

/**
 * @tc.name: UsbRequestInitialize003
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize003 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize003 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize003 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize003 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request read";
    std::vector<uint8_t> cdata = {tag, tag + 12};
    request.SetClientData(cdata);
    pip.SetBusNum(255);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize003 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize003 : Initialize");
}

/**
 * @tc.name: UsbRequestInitialize004
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize004 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize004 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize004 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize004 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request read";
    std::vector<uint8_t> cdata = {tag, tag + 12};
    request.SetClientData(cdata);
    pip.SetDevAddr(255);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize004 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize004 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize004 : Initialize");
}

/**
 * @tc.name: UsbRequestInitialize005
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize005 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize005 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize005 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize005 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request write";
    std::vector<uint8_t> cdata = {tag, tag + 13};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> bufferdata = {buffer, buffer + 17};
    request.SetReqData(bufferdata);
    pip.SetBusNum(255);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize005 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize005 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize005 : Initialize");
}

/**
 * @tc.name: UsbRequestInitialize006
 * @tc.desc: Test functions of Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbRequestInitialize006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbRequestInitialize006 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize006 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize006 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize006 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request write";
    std::vector<uint8_t> cdata = {tag, tag + 13};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> bufferdata = {buffer, buffer + 17};
    request.SetReqData(bufferdata);
    pip.SetDevAddr(255);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbRequestInitialize006 %{public}d ret=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbRequestInitialize006 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbRequestInitialize006 : Initialize");
}

/**
 * @tc.name: UsbrequestQueue001
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue001 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue read001";
    std::vector<uint8_t> cdata = {buffer, buffer + 21};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue001 : RequestQueue");
}

/**
 * @tc.name: UsbrequestQueue002
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue002 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue002 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue002 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue write 002";
    std::vector<uint8_t> cdata = {buffer, buffer + 23};
    request.SetClientData(cdata);
    uint8_t buffer1[255] = "usb request test!";
    std::vector<uint8_t> bufferdata = {buffer1, buffer1 + 17};
    request.SetReqData(bufferdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbrequestQueue002 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:UsbrequestQueue002 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue002 : RequestQueue");
}

/**
 * @tc.name: UsbrequestQueue003
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue003 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue read003";
    std::vector<uint8_t> cdata = {buffer, buffer + 21};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue003 : RequestQueue");
}

/**
 * @tc.name: UsbrequestQueue004
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue004 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue read004";
    std::vector<uint8_t> cdata = {buffer, buffer + 21};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue004 : RequestQueue");
}

/**
 * @tc.name: UsbrequestQueue005
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue005 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue write005";
    std::vector<uint8_t> cdata = {buffer, buffer + 22};
    request.SetClientData(cdata);
    uint8_t buffer1[255] = "usb request test!";
    std::vector<uint8_t> databuffer = {buffer1, buffer1 + 17};
    request.SetReqData(databuffer);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue005 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue005 : RequestQueue");
}

/**
 * @tc.name: UsbrequestQueue006
 * @tc.desc: Test functions of RequestQueue(UsbRequest &request, uint8_t *buffer, uint32_t length)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, UsbrequestQueue006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbrequestQueue006 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request queue write006";
    std::vector<uint8_t> cdata = {buffer, buffer + 22};
    request.SetClientData(cdata);
    uint8_t buffer1[255] = "usb request test!";
    std::vector<uint8_t> databuffer = {buffer1, buffer1 + 17};
    request.SetReqData(databuffer);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::UsbrequestQueue006 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbrequestQueue006 : RequestQueue");
}

/**
 * @tc.name: Usbrequestfree001
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree001 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree001 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree001 %{public}d OpenDevice=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 001";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree001 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree001 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree001 : RequestFree");
}

/**
 * @tc.name: Usbrequestfree002
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree002 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree002 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree002 %{public}d OpenDevice=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 002";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree002 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree002 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree002 : RequestFree");
}

/**
 * @tc.name: Usbrequestfree003
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree003 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree003 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree003 %{public}d OpenDevice=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 003";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree003 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree003 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree003 : RequestFree");
}

/**
 * @tc.name: Usbrequestfree004
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree004 : Requestfree");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree004 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree004 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    UsbRequest request;
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree004 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 004";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> databuffer = {buffer, buffer + 17};
    request.SetReqData(databuffer);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree004 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree004 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree004 : Requestfree");
}

/**
 * @tc.name: Usbrequestfree005
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree005 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree005 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree005 %{public}d OpenDevice=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree005 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 005";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> databuffer = {buffer, buffer + 17};
    request.SetReqData(databuffer);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree005 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree005 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree005 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree005 : RequestFree");
}

/**
 * @tc.name: Usbrequestfree006
 * @tc.desc: Test functions of RequestFree(UsbRequest &request);
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestfree006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestfree006 : RequestQueue");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree006 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree006 %{public}d OpenDevice=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree006 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request free 006";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    uint8_t buffer[255] = "usb request test!";
    std::vector<uint8_t> databuffer = {buffer, buffer + 17};
    request.SetReqData(databuffer);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree006 %{public}d initialize=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestFree(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestfree006 %{public}d free=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestfree006 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestfree006 : RequestFree");
}

/**
 * @tc.name: Usbrequestabort001
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort001 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort001 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort001 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "hello world request013";
    std::vector<uint8_t> cdata = {tag, tag + 22};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort001 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request abort 001";
    std::vector<uint8_t> databuffer = {buffer, buffer + 17};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort001 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort001 %{public}d Abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort001 : Requestabort");
}

/**
 * @tc.name: Usbrequestabort002
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort002 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort002 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort002 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request abort 002";
    std::vector<uint8_t> cdata = {tag, tag + 17};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort002 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort002 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort002 %{public}d Abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort002 : Requestabort");
}

/**
 * @tc.name: Usbrequestabort003
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort003 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort003 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort003 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request abort 003";
    std::vector<uint8_t> cdata = {tag, tag + 17};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort003 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort003 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort003 %{public}d Abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort003 : Requestabort");
}

/**
 * @tc.name: Usbrequestabort004
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort004 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d GetDirection=%{public}d ", __LINE__,
               point.GetDirection());
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d GetInterfaceId=%{public}d ", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "hello world request013";
    std::vector<uint8_t> cdata = {tag, tag + 22};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "request abort 004";
    std::vector<uint8_t> databuffer = {buffer, buffer + 17};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort004 %{public}d abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort004 : Requestabort");
}

/**
 * @tc.name: Usbrequestabort005
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort005 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort005 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort005 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request abort 005";
    std::vector<uint8_t> cdata = {tag, tag + 17};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "usb request test 005";
    std::vector<uint8_t> databuffer = {buffer, buffer + 20};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort005 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetBusNum(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort005 %{public}d Abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort005 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort005 : Requestabort");
}

/**
 * @tc.name: Usbrequestabort006
 * @tc.desc: Test functions of RequestAbort(request)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, Usbrequestabort006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestabort006 : Requestabort");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort006 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort006 %{public}d OpenDevice=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    EXPECT_TRUE(ret == 0);
    uint8_t tag[50] = "request abort 006";
    std::vector<uint8_t> cdata = {tag, tag + 17};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "usb request test 006";
    std::vector<uint8_t> databuffer = {buffer, buffer + 20};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort006 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pip.SetDevAddr(255);
    request.SetPipe(pip);
    ret = UsbSrvClient.RequestAbort(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort006 %{public}d Abort=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::Usbrequestabort006 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestabort006 : Requestabort");
}

/**
 * @tc.name: PipeRequestWait001
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait001 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait001 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request wait 001";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:Usbrequestabort006 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait001 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait001 : Initialize");
}

/**
 * @tc.name: PipeRequestWait002
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait002 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait002 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request wait 002";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait002 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    pip.SetBusNum(255);
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait002 : Initialize");
}

/**
 * @tc.name: PipeRequestWait003
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait003 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait003 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request wait 003";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait003 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait003 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    pip.SetDevAddr(255);
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait003 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait003 : Initialize");
}

/**
 * @tc.name: PipeRequestWait004
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait004 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait004 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait004 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request wait 004";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait004 %{public}d initialize=%{public}d ", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "usb request wait test004";
    std::vector<uint8_t> databuffer = {buffer, buffer + 24};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait004 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait004 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait004 : Initialize");
}

/**
 * @tc.name: PipeRequestWait005
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait005 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait005 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait005 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request wait 005";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "usb request wait test005";
    std::vector<uint8_t> databuffer = {buffer, buffer + 24};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait005 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    pip.SetBusNum(255);
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait005 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pip.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait005 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait005 : Initialize");
}

/**
 * @tc.name: PipeRequestWait006
 * @tc.desc: Test functions of PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req)
 * @tc.type: FUNC
 */
HWTEST_F(UsbRequestTest, PipeRequestWait006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : PipeRequestWait006 : Initialize");
    vector<UsbDevice> delist;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(delist);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest:PipeRequestWait006 %{public}d ret=%{public}d size:%{public}d",
               __LINE__, ret, delist.size());
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(delist.empty())) << "delist NULL";
    UsbDevice device = delist.front();
    UsbSrvClient.RequestRight(device.GetName());
    USBDevicePipe pip;
    ret = UsbSrvClient.OpenDevice(device, pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait006 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    ret = UsbSrvClient.ClaimInterface(pip, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait006 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest request;
    uint8_t tag[50] = "request abort 006";
    std::vector<uint8_t> cdata = {tag, tag + 16};
    request.SetClientData(cdata);
    ret = request.Initialize(pip, point);
    EXPECT_TRUE(ret == 0);
    uint8_t buffer[255] = "usb request wait test006";
    std::vector<uint8_t> databuffer = {buffer, buffer + 24};
    request.SetReqData(databuffer);
    ret = UsbSrvClient.RequestQueue(request);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait006 %{public}d queue=%{public}d ", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbRequest reqwait;
    pip.SetDevAddr(255);
    ret = UsbSrvClient.PipeRequestWait(pip, 5000, reqwait);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait006 %{public}d PipeRequestWait=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pip.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pip);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbRequestTest::PipeRequestWait006 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : PipeRequestWait006 : Initialize");
}