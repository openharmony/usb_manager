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

#include <gtest/gtest.h>
#include <json/json.h>
#include <semaphore.h>

#include <iostream>

#include "common_event_manager.h"
#include "common_event_support.h"
#include "usb_srv_client.h"
#include "usb_srv_support.h"

using namespace OHOS::EventFwk;
using namespace testing::ext;
using namespace OHOS::USB;

namespace OHOS {
namespace USB {
class UsbEventTest : public testing::Test {
public:
    void SetUp() override;
    void TearDown() override;

    static sem_t testSem_;
};

sem_t UsbEventTest::testSem_ {};

class UsbSubscriberTest : public CommonEventSubscriber {
public:
    explicit UsbSubscriberTest(const CommonEventSubscribeInfo &sp) : CommonEventSubscriber(sp) {}

    void OnReceiveEvent(const CommonEventData &data) override
    {
        eventData_ = data;
        sem_post(&UsbEventTest::testSem_);
    }

    static CommonEventData eventData_;
};

CommonEventData UsbSubscriberTest::eventData_ {};

void UsbEventTest::SetUp()
{
    sem_init(&UsbEventTest::testSem_, 1, 0);
}

void UsbEventTest::TearDown()
{
    sem_destroy(&UsbEventTest::testSem_);
}

/**
 * @tc.name: SUB_USB_Broadcast_0100
 * @tc.desc: usb gadget connected event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0100, TestSize.Level1)
{
    // subscribe usb common event
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_STATE);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);

    std::cout << "please connect the gadget to some host" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_STATE);
    EXPECT_TRUE(want.GetBoolParam(std::string {UsbSrvSupport::CONNECTED}, false));
    EXPECT_TRUE(want.GetBoolParam(std::string {UsbSrvSupport::FUNCTION_NAME_HDC}, false));
}

/**
 * @tc.name: SUB_USB_Broadcast_0200
 * @tc.desc: usb gadget disconnected event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0200, TestSize.Level1)
{
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_STATE);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);

    std::cout << "please disconnect the gadget" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_STATE);
    EXPECT_FALSE(want.GetBoolParam(std::string {UsbSrvSupport::CONNECTED}, true));
    EXPECT_TRUE(want.GetBoolParam(std::string {UsbSrvSupport::FUNCTION_NAME_HDC}, false));
}

/**
 * @tc.name: SUB_USB_Broadcast_0300
 * @tc.desc: usb device attached event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0300, TestSize.Level1)
{
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);

    std::cout << "please connect a device to the host" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_DEVICE_ATTACHED);

    // parse string to Json::Value
    std::string deviceStr = UsbSubscriberTest::eventData_.GetData();
    std::cout << deviceStr << std::endl;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value deviceJson;
    auto parseResult = reader->parse(deviceStr.c_str(), deviceStr.c_str() + deviceStr.length(), &deviceJson, &err);
    EXPECT_TRUE(parseResult);

    // valid device
    UsbDevice device(deviceJson);
    EXPECT_NE(device.GetiProduct(), 0);
    EXPECT_NE(device.GetiManufacturer(), 0);
    EXPECT_GT(device.GetConfigCount(), 0);

    // valid config
    USBConfig config;
    device.GetConfig(0, config);
    EXPECT_NE(config.GetAttributes(), 0);
    EXPECT_GT(config.GetInterfaceCount(), 0);

    // valid interface
    UsbInterface interface;
    config.GetInterface(0, interface);
    EXPECT_NE(interface.GetClass(), 0);
    EXPECT_NE(interface.GetEndpointCount(), 0);

    // valid endpoint
    auto endpoint = interface.GetEndpoint(0);
    EXPECT_NE(endpoint.value().GetAttributes(), 0);
}

/**
 * @tc.name: SUB_USB_Broadcast_0400
 * @tc.desc: usb device detached event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0400, TestSize.Level1)
{
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);

    std::cout << "please disconnect the device to the host" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_DEVICE_DETACHED);

    std::string deviceStr = UsbSubscriberTest::eventData_.GetData();
    std::cout << deviceStr << std::endl;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value deviceJson;
    auto parseResult = reader->parse(deviceStr.c_str(), deviceStr.c_str() + deviceStr.length(), &deviceJson, &err);
    EXPECT_TRUE(parseResult);

    // valid device
    UsbDevice device(deviceJson);
    EXPECT_NE(device.GetiProduct(), 0);
    EXPECT_NE(device.GetiManufacturer(), 0);
}

/**
 * @tc.name: SUB_USB_Broadcast_0500
 * @tc.desc: usb port change to host event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0500, TestSize.Level1)
{
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);
    std::cout << "please switch port to host" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);

    std::string portStr = UsbSubscriberTest::eventData_.GetData();
    std::cout << portStr << std::endl;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value portJson;
    auto parseResult = reader->parse(portStr.c_str(), portStr.c_str() + portStr.length(), &portJson, &err);
    EXPECT_TRUE(parseResult);

    // valid event
    EXPECT_EQ(portJson["mode"].asInt(), UsbSrvSupport::PORT_MODE_HOST);
}

/**
 * @tc.name: SUB_USB_Broadcast_0600
 * @tc.desc: usb port change to device event
 * @tc.type: FUNC
 */
HWTEST_F(UsbEventTest, SUB_USB_Broadcast_0600, TestSize.Level1)
{
    MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);
    CommonEventSubscribeInfo subscriberInfo(matchingSkills);
    std::shared_ptr<UsbSubscriberTest> subscriber = std::make_shared<UsbSubscriberTest>(subscriberInfo);
    CommonEventManager::SubscribeCommonEvent(subscriber);
    std::cout << "please switch port to device" << std::endl;

    // block until UsbSubscriberTest post
    sem_wait(&UsbEventTest::testSem_);
    CommonEventManager::UnSubscribeCommonEvent(subscriber);

    auto &want = UsbSubscriberTest::eventData_.GetWant();
    EXPECT_EQ(want.GetAction(), CommonEventSupport::COMMON_EVENT_USB_PORT_CHANGED);

    std::string portStr = UsbSubscriberTest::eventData_.GetData();
    std::cout << portStr << std::endl;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value portJson;
    auto parseResult = reader->parse(portStr.c_str(), portStr.c_str() + portStr.length(), &portJson, &err);
    EXPECT_TRUE(parseResult);

    // valid event
    EXPECT_EQ(portJson["mode"].asInt(), UsbSrvSupport::PORT_MODE_DEVICE);
}
} // namespace USB
} // namespace OHOS
