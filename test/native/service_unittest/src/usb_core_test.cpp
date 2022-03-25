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

#include "usb_core_test.h"
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include "hilog_wrapper.h"
#include "if_system_ability_manager.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "string_ex.h"
#include "system_ability_definition.h"
#include "usb_errors.h"
#include "usb_service.h"
#include "usb_srv_client.h"

using namespace testing::ext;
using namespace OHOS::USB;
using namespace OHOS;
using namespace std;

const int32_t SLEEP_TIME = 3;

const int32_t TEST_PORT_ID = 1;
const int32_t TEST_POWER_ROLE = 2;
const int32_t TEST_DATAR_ROLE = 2;

void UsbCoreTest::SetUpTestCase(void)
{
    auto &srvClient = UsbSrvClient::GetInstance();
    auto ret = srvClient.SetPortRole(TEST_PORT_ID, TEST_POWER_ROLE, TEST_DATAR_ROLE);
    sleep(SLEEP_TIME);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest:: [Device] SetPortRole=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    if (ret != 0) {
        exit(0);
    }
    USB_HILOGI(MODULE_USB_SERVICE, "Start UsbCoreTest");
}

void UsbCoreTest::TearDownTestCase(void)
{
    USB_HILOGI(MODULE_USB_SERVICE, "End UsbCoreTest");
}

void UsbCoreTest::SetUp(void) {}

void UsbCoreTest::TearDown(void) {}

/**
 * @tc.name: GetCurrentFunctions001
 * @tc.desc: Test functions to GetCurrentFunctions()
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, GetCurrentFunctions001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetCurrentFunctions001 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t funcs = 0;
    funcs = instance.GetCurrentFunctions(funcs);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::funcs=%{public}d", funcs);
    ASSERT_TRUE(funcs == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetCurrentFunctions001 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions001
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions001 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions001 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions002
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions002 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(2);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions002 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions003
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions003 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(3);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions003 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions004
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions004 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(4);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions004 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions005
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions005 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions005 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions006
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions006 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(6);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions006 : SetConfig");
}

/**
 * @tc.name: SetCurrentFunctions007
 * @tc.desc: Test functions to SetCurrentFunctions(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetCurrentFunctions007, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetCurrentFunctions007 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t isok = instance.SetCurrentFunctions(8);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::SetCurrentFunctions=%{public}d", isok);
    ASSERT_TRUE(isok != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetCurrentFunctions007 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString001
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString001 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t funcCode = instance.UsbFunctionsFromString(UsbFunctionManager::FUNCTION_NAME_NONE);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode != UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString001 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString002
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString002 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t funcCode = instance.UsbFunctionsFromString(UsbFunctionManager::FUNCTION_NAME_HDC);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode != UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString002 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString003
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString003 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t funcCode = instance.UsbFunctionsFromString(UsbFunctionManager::FUNCTION_NAME_ACM);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode != UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString003 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString004
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString004 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    int32_t funcCode = instance.UsbFunctionsFromString(UsbFunctionManager::FUNCTION_NAME_ECM);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode != UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString004 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString005
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString005 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcs = "qwerts";
    int32_t funcCode = instance.UsbFunctionsFromString(funcs);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode == UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString005 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsFromString006
 * @tc.desc: Test functions to UsbFunctionsFromString(string funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsFromString006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsFromString006 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcs = "zxcbvx";
    int32_t funcCode = instance.UsbFunctionsFromString(funcs);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::functionsFromString=%{public}d", funcCode);
    ASSERT_TRUE(funcCode == UEC_SERVICE_INVALID_VALUE);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsFromString006 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsToString001
 * @tc.desc: Test functions to UsbFunctionsToString(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsToString001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsToString001 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcName = instance.UsbFunctionsToString(UsbFunctionManager::FUNCTION_NONE);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbFunctionsToString=%{public}s", funcName.c_str());
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbFunctionsToString=%{public}d", funcName.size());
    ASSERT_TRUE(!(funcName.empty()));
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsToString001 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsToString002
 * @tc.desc: Test functions to UsbFunctionsToString(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsToString002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsToString002 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcName = instance.UsbFunctionsToString(UsbFunctionManager::FUNCTION_HDC);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbFunctionsToString=%{public}s", funcName.c_str());
    ASSERT_TRUE(!(funcName.empty()));
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsToString002 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsToString003
 * @tc.desc: Test functions to UsbFunctionsToString(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsToString003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsToString003 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcName = instance.UsbFunctionsToString(UsbFunctionManager::FUNCTION_ACM);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbFunctionServiceTest::UsbFunctionsToString=%{public}s", funcName.c_str());
    ASSERT_TRUE(!(funcName.empty()));
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsToString003 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsToString004
 * @tc.desc: Test functions to UsbFunctionsToString(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsToString004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsToString004 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcName = instance.UsbFunctionsToString(UsbFunctionManager::FUNCTION_ECM);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbFunctionServiceTest::UsbFunctionsToString=%{public}s", funcName.c_str());
    ASSERT_TRUE(!(funcName.empty()));
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsToString004 : SetConfig");
}

/**
 * @tc.name: UsbFunctionsToString005
 * @tc.desc: Test functions to UsbFunctionsToString(int32_t funcs)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbFunctionsToString005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbFunctionsToString005 : SetConfig");
    auto &instance = UsbSrvClient::GetInstance();
    std::string funcName = instance.UsbFunctionsToString(-1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbFunctionServiceTest::UsbFunctionsToString=%{public}s", funcName.c_str());
    ASSERT_TRUE(!(funcName.empty()));
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbFunctionsToString005 : SetConfig");
}

/**
 * @tc.name: UsbHasRight001
 * @tc.desc: Test functions of HasRight
 * @tc.desc: int32_t HasRight(std::string deviceName)
 * @tc.desc: 正向测试：代码正常运行，返回结果为-1
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbHasRight001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbHasRight001: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_80";
    int32_t result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight001 HasRight=%{public}d", result);
    ASSERT_TRUE(result != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbHasRight001: SetConfig");
}

/**
 * @tc.name: UsbHasRight002
 * @tc.desc: Test functions of HasRight
 * @tc.desc: int32_t HasRight(std::string deviceName)
 * @tc.desc: 反向测试：代码正常运行，返回结果为0
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbHasRight002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbHasRight002: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_80";
    int32_t result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight002 HasRight=%{public}d", result);
    ASSERT_TRUE(result != 0);
    result = UsbSrvClient.RequestRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight002 RequestRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight002 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbHasRight002: SetConfig");
}

/**
 * @tc.name: UsbHasRight003
 * @tc.desc: Test functions of HasRight
 * @tc.desc: int32_t HasRight(std::string deviceName)
 * @tc.desc: 反向测试：代码正常运行，返回结果为-1
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbHasRight003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbHasRight003: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_80";
    int32_t result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight003 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.RemoveRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight003 RemoveRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    deviceName = "device_81";
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight003 HasRight=%{public}d", result);
    ASSERT_TRUE(result != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbHasRight003: SetConfig");
}

/**
 * @tc.name: UsbHasRight004
 * @tc.desc: Test functions of HasRight
 * @tc.desc: int32_t HasRight(std::string deviceName)
 * @tc.desc: 反向测试：代码正常运行，返回结果为0
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, UsbHasRight004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : UsbHasRight004: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_82";
    int32_t result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight004 HasRight=%{public}d", result);
    ASSERT_TRUE(result != 0);
    result = UsbSrvClient.RequestRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight004 RequestRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::UsbHasRight004 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.RemoveRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 RemoveRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : UsbHasRight004: SetConfig");
}

/**
 * @tc.name: Usbrequestright001
 * @tc.desc: Test functions of requestright
 * @tc.desc: int32_t requestright(std::string deviceName)
 * @tc.desc: 正向测试：代码正常运行
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, Usbrequestright001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestright001: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_83";
    int32_t result = UsbSrvClient.RequestRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright001 RequestRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright001 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.RemoveRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 RemoveRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestright001: SetConfig");
}
/**
 * @tc.name: Usbrequestright002
 * @tc.desc: Test functions of requestright
 * @tc.desc: int32_t requestright(std::string deviceName)
 * @tc.desc: 正向测试：代码正常运行
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, Usbrequestright002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestright002: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_84";
    int32_t result = UsbSrvClient.RequestRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright002 RequestRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright002 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.RemoveRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 RemoveRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestright002: SetConfig");
}

/**
 * @tc.name: Usbrequestright003
 * @tc.desc: Test functions of requestright
 * @tc.desc: int32_t requestright(std::string deviceName)
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, Usbrequestright003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : Usbrequestright003: SetConfig");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    std::string deviceName = "device_81";
    int32_t result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 HasRight=%{public}d", result);
    ASSERT_TRUE(result != 0);
    result = UsbSrvClient.RequestRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 RequestRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.HasRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 HasRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    result = UsbSrvClient.RemoveRight(deviceName);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::Usbrequestright003 RemoveRight=%{public}d", result);
    ASSERT_TRUE(result == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : Usbrequestright003: SetConfig");
}

/**
 * @tc.name: GetPorts001
 * @tc.desc: Test functions to GetPorts
 * @tc.desc: int32_t GetPorts(std::vector<UsbPort *> &usbports);
 * @tc.desc: 正向测试：参数正确
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, GetPorts001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetPorts001 : GetPorts");
    auto &usbSrvClient = UsbSrvClient::GetInstance();
    std::vector<UsbPort> portlist;
    auto ports = usbSrvClient.GetPorts(portlist);
    USB_HILOGD(MODULE_USB_SERVICE, "Get UsbPort size=%{public}d", portlist.size());
    ASSERT_TRUE(ports == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetPorts001 : GetPorts");
}

/**
 * @tc.name: GetSupportedModes001
 * @tc.desc: Test functions to GetSupportedModes
 * @tc.desc: int32_t GetSupportedModes(int32_t portId, int32_t &result);
 * @tc.desc: 正向测试：参数正确
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, GetSupportedModes001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetSupportedModes001 : GetSupportedModes");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    int32_t result = 0;
    auto modes = UsbSrvClient.GetSupportedModes(0, result);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", modes);
    ASSERT_TRUE(modes != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetSupportedModes001 : GetSupportedModes");
}

/**
 * @tc.name: GetSupportedModes002
 * @tc.desc: Test functions to GetSupportedModes
 * @tc.desc: int32_t GetSupportedModes(int32_t portId, int32_t &result);
 * @tc.desc: 反向测试：portid错误
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, GetSupportedModes002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetSupportedModes002 : GetSupportedModes");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    int32_t result = 0;
    auto modes = UsbSrvClient.GetSupportedModes(-1, result);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", modes);
    ASSERT_TRUE(modes != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetSupportedModes002 : GetSupportedModes");
}

/**
 * @tc.name: GetSupportedModes003
 * @tc.desc: Test functions to GetSupportedModes
 * @tc.desc: int32_t GetSupportedModes(int32_t portId, int32_t &result);
 * @tc.desc: 反向测试：portid错误
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, GetSupportedModes003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetSupportedModes003 : GetSupportedModes");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    int32_t result = 0;
    auto modes = UsbSrvClient.GetSupportedModes(0xFFFFFFFF, result);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", modes);
    ASSERT_TRUE(modes != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetSupportedModes003 : GetSupportedModes");
}

/**
 * @tc.name: GetSupportedModes004
 * @tc.desc: Test functions to GetSupportedModes
 * @tc.desc: int32_t GetSupportedModes(int32_t portId, int32_t &result);
 * @tc.desc: 正向测试：参数正确
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, GetSupportedModes004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : GetSupportedModes004 : GetSupportedModes");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    int32_t result = 0;
    auto modes = UsbSrvClient.GetSupportedModes(1, result);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", modes);
    ASSERT_TRUE(modes == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : GetSupportedModes004 : GetSupportedModes");
}

/**
 * @tc.name: SetPortRole001
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 正向测试：参数正确
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, SetPortRole001, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole001 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(1, 1, 1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole001 : SetPortRole");
}

/**
 * @tc.name: SetPortRole002
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：portid错误
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, SetPortRole002, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole002 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(2, 1, 1);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole002 : SetPortRole");
}

/**
 * @tc.name: SetPortRole003
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：powerRole错误
 * @tc.type: FUNC
 */

HWTEST_F(UsbCoreTest, SetPortRole003, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole003 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(1, 4, 2);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole003 : SetPortRole");
}

/**
 * @tc.name: SetPortRole004
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：dataRole错误
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetPortRole004, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole004 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(1, 1, 5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole004 : SetPortRole");
}

/**
 * @tc.name: SetPortRole005
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：参数异常，portId、powerRole错误
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetPortRole005, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole005 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(1, 5, 5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole005 : SetPortRole");
}

/**
 * @tc.name: SetPortRole006
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：powerRole、dataRole错误
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetPortRole006, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole006 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(5, 1, 5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole006 : SetPortRole");
}

/**
 * @tc.name: SetPortRole007
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc: 反向测试：powerRole、dataRole错误
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetPortRole007, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole007 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(2, 5, 5);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret != 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole007 : SetPortRole");
}

/**
 * @tc.name: SetPortRole008
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole);
 * @tc.desc:
 * @tc.type: FUNC
 */
HWTEST_F(UsbCoreTest, SetPortRole008, TestSize.Level1)
{
    USB_HILOGI(MODULE_USB_SERVICE, "Case Start : SetPortRole008 : SetPortRole");
    auto &UsbSrvClient = UsbSrvClient::GetInstance();
    auto ret = UsbSrvClient.SetPortRole(1, 2, 2);
    USB_HILOGI(MODULE_USB_SERVICE, "UsbCoreTest::status=%{public}d", ret);
    ASSERT_TRUE(ret == 0);
    USB_HILOGI(MODULE_USB_SERVICE, "Case End : SetPortRole008 : SetPortRole");
}
