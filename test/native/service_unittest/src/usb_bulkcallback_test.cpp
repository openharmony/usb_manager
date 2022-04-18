/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "usb_bulkcallback_test.h"
#include <iostream>
#include <vector>
#include <sys/time.h>
#include "delayed_sp_singleton.h"
#include "hilog_wrapper.h"
#include "if_system_ability_manager.h"
#include "system_ability_definition.h"
#include "usb_srv_client.h"
#include "ashmem.h"
#include "securec.h"
#include "usb_errors.h"
#include "iservice_registry.h"
#include "usb_callback_test.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;

const int32_t SLEEP_TIME = 3;

const uint32_t ASHMEM_MAX_SIZE = 1024;
const uint32_t MEM_DATA = 1024 * 1024;

int32_t InitAshmemOne(sptr<Ashmem> &asmptr, int32_t asmSize, uint8_t rflg)
{
    asmptr = Ashmem::CreateAshmem("ttashmem000", asmSize);
    if (asmptr == nullptr) {
        USB_HILOGE(MODULE_USB_SERVICE, "InitAshmemOne CreateAshmem failed\n");
        return UEC_SERVICE_NO_MEMORY;
    }

    asmptr->MapReadAndWriteAshmem();

    if (rflg == 0) {
        uint8_t tdata[ASHMEM_MAX_SIZE];
        int32_t offset = 0;
        int32_t tlen = 0;

        int32_t retSafe = memset_s(tdata, sizeof(tdata), 'Y', ASHMEM_MAX_SIZE);
        if (retSafe != EOK) {
            USB_HILOGE(MODULE_USB_SERVICE, "InitAshmemOne memset_s failed\n");
            return UEC_SERVICE_NO_MEMORY;
        }
        while (offset < asmSize) {
            tlen = (asmSize - offset) < ASHMEM_MAX_SIZE ? (asmSize - offset) : ASHMEM_MAX_SIZE;
            asmptr->WriteToAshmem(tdata, tlen, offset);
            offset += tlen;
        }
    }

    return 0;
}

void UsbBulkcallbackTest::SetUpTestCase(void)
{
    auto &srvClient = UsbSrvClient::GetInstance();
    auto ret = srvClient.SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest:: [Device] SetPortRole=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    if (ret != 0) {
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int32_t c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Start UsbBulkcallbackTest");
}

void UsbBulkcallbackTest::TearDownTestCase(void)
{
    USB_HILOGI(MODULE_USB_SERVICE, "End UsbBulkcallbackTest");
}

void UsbBulkcallbackTest::SetUp(void) {}

void UsbBulkcallbackTest::TearDown(void) {}

/**
 * @tc.name: RegBulkCallback001
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, RegBulkCallback001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback001 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback001 : BulkTransfer");
}

/**
 * @tc.name: RegBulkCallback002
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */

HWTEST_F(UsbBulkcallbackTest, RegBulkCallback002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback002 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback002 : BulkTransfer");
}

/**
 * @tc.name: RegBulkCallback003
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */

HWTEST_F(UsbBulkcallbackTest, RegBulkCallback003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback003 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback003 : BulkTransfer");
}

/**
 * @tc.name: RegBulkCallback004
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */

HWTEST_F(UsbBulkcallbackTest, RegBulkCallback004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback004 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback004 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback004 : BulkTransfer");
}

/**
 * @tc.name: RegBulkCallback005
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */

HWTEST_F(UsbBulkcallbackTest, RegBulkCallback005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback005 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback005 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback005 : BulkTransfer");
}

/**
 * @tc.name: RegBulkCallback006
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe, const
 * sptr<IRemoteObject> &cb)
 * @tc.type: FUNC
 */

HWTEST_F(UsbBulkcallbackTest, RegBulkCallback006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : RegBulkCallback006 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(0);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::RegBulkCallback006 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : RegBulkCallback006 : BulkTransfer");
}

/**
 * @tc.name: BulkRead001
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkRead001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkRead001 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkRead001 : BulkTransfer");
}

/**
 * @tc.name: BulkRead002
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkRead002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkRead002 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkRead002 : BulkTransfer");
}

/**
 * @tc.name: BulkRead003
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkRead003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkRead003 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkRead003 : BulkTransfer");
}

/**
 * @tc.name: BulkRead004
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkRead004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkRead004 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d OpenDevice=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkRead004 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkRead004 : BulkTransfer");
}

/**
 * @tc.name: BulkWrite001
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkWrite001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkWrite001 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkWrite(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkWrite001 : BulkTransfer");
}

/**
 * @tc.name: BulkWrite002
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkWrite002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkWrite002 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.BulkWrite(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkWrite002 : BulkTransfer");
}

/**
 * @tc.name: BulkWrite003
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &devInfo, const UsbPipe &pipe, sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkWrite003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkWrite003 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().at(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d ClaimInterface=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d RegBulkCallback=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.BulkWrite(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkWrite003 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkWrite003 : BulkTransfer");
}

/**
 * @tc.name: BulkCancel001
 * @tc.desc: Test functions to int32_t BulkCancel(const UsbDev &devInfo, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkCancel001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkCancel001 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel001 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel001 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel001 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel001 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel001 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkCancel001 : BulkTransfer");
}

/**
 * @tc.name: BulkCancel002
 * @tc.desc: Test functions to int32_t BulkCancel(const UsbDev &devInfo, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, BulkCancel002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : BulkCancel002 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MEM_DATA;
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    ret = InitAshmemOne(ashmem, asmSize, rflg);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d ret=%{public}d", __LINE__, ret);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d OpenDevice=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.BulkRead(pipe, point, ashmem);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d BulkRead=%{public}d", __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    ret = UsbSrvClient.BulkCancel(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d BulkCancel=%{public}d", __LINE__,
               ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::BulkCancel002 %{public}d close=%{public}d", __LINE__, close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : BulkCancel002 : BulkTransfer");
}

/**
 * @tc.name: UnRegBulkCallback001
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, UnRegBulkCallback001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UnRegBulkCallback001 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback001 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UnRegBulkCallback001 : BulkTransfer");
}

/**
 * @tc.name: UnRegBulkCallback002
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, UnRegBulkCallback002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UnRegBulkCallback002 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetBusNum(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetBusNum(device.GetBusNum());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback002 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UnRegBulkCallback002 : BulkTransfer");
}

/**
 * @tc.name: UnRegBulkCallback003
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &devInfo, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbBulkcallbackTest, UnRegBulkCallback003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UnRegBulkCallback003 : BulkTransfer");
    vector<UsbDevice> devi;
    sptr<UsbCallbackTest> cb = new UsbCallbackTest();
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.GetDevices(devi);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(!(devi.empty())) << "delist NULL";
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d size=%{public}d", __LINE__,
               devi.size());
    USBDevicePipe pipe;
    UsbDevice device = devi.front();
    UsbSrvClient.RequestRight(device.GetName());
    ret = UsbSrvClient.OpenDevice(device, pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d OpenDevice=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    UsbInterface interface = device.GetConfigs().front().GetInterfaces().at(1);
    USBEndpoint point = interface.GetEndpoints().front();
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d point=%{public}d", __LINE__,
               point.GetInterfaceId());
    ret = UsbSrvClient.ClaimInterface(pipe, interface, true);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d ClaimInterface=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret == 0);
    pipe.SetDevAddr(255);
    ret = UsbSrvClient.RegBulkCallback(pipe, point, cb);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d RegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    ret = UsbSrvClient.UnRegBulkCallback(pipe, point);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d UnRegBulkCallback=%{public}d",
               __LINE__, ret);
    EXPECT_TRUE(ret != 0);
    pipe.SetDevAddr(device.GetDevAddr());
    bool close = UsbSrvClient.Close(pipe);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbBulkcallbackTest::UnRegBulkCallback003 %{public}d close=%{public}d", __LINE__,
               close);
    EXPECT_TRUE(close);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UnRegBulkCallback003 : BulkTransfer");
}