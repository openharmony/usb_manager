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

#include "usb_srv_client.h"

using namespace std;
using namespace OHOS;
using namespace OHOS::USB;

static const int32_t DEFAULT_PORT_ID = 1;
static const int32_t DEFAULT_ROLE_HOST = 1;
static const int32_t DEFAULT_ROLE_DEVICE = 2;

static const int32_t HOST_MODE = 2;

static UsbSrvClient &g_usbClient = UsbSrvClient::GetInstance();

static void PrintHelp()
{
    printf("2 args\n");
    printf("-p 0: Query Port\n");
    printf("-p 1: Switch to host\n");
    printf("-p 2: Switch to device:\n");
    printf("-f 0: Query fucntion\n");
    printf("-f 1: Switch to function:acm\n");
    printf("-f 2: Switch to function:ecm\n");
    printf("-f 3: Switch to function:acm&ecm\n");
    printf("-f 4: Switch to function:hdc\n");
    printf("-f 5: Switch to function:acm&hdc\n");
    printf("-f 6: Switch to function:ecm&hdc\n");
}

static void GetCurrentFunctionInfo()
{
    int32_t funcs = -1;
    string strFun = "";
    int32_t ret = g_usbClient.GetCurrentFunctions(funcs);
    if (ret) {
        printf("%s:%d error exit\n", __func__, __LINE__);
        return;
    }
    strFun = g_usbClient.UsbFunctionsToString(funcs);
    printf("%s:%d get current function: %s\n", __func__, __LINE__, strFun.c_str());
}

static void FunctionSwitch(UsbSrvClient &g_usbClient, int32_t mode)
{
    switch (mode) {
        case 0:
            GetCurrentFunctionInfo();
            break;
        default:
            int32_t ret = g_usbClient.SetCurrentFunctions(mode);
            if (ret) {
                printf("%s:%d error exit\n", __func__, __LINE__);
                break;
            }
            GetCurrentFunctionInfo();
            break;
    }
}

static void GetPortsInfo()
{
    std::vector<UsbPort> usbports;
    int32_t ret = g_usbClient.GetPorts(usbports);
    if (ret) {
        printf("%s:%d error exit\n", __func__, __LINE__);
        return;
    }

    if (usbports[0].usbPortStatus.currentMode == HOST_MODE) {
        printf("get current port %d: host\n", usbports[0].usbPortStatus.currentMode);
    } else {
        printf("get current port %d: device\n", usbports[0].usbPortStatus.currentMode);
    }
}

static void PortSwitch(UsbSrvClient &g_usbClient, int32_t mode)
{
    switch (mode) {
        case 0:
            GetPortsInfo();
            break;
        case DEFAULT_ROLE_HOST:
            g_usbClient.SetPortRole(DEFAULT_PORT_ID, DEFAULT_ROLE_HOST, DEFAULT_ROLE_HOST);
            GetPortsInfo();
            break;
        case DEFAULT_ROLE_DEVICE:
            g_usbClient.SetPortRole(DEFAULT_PORT_ID, DEFAULT_ROLE_DEVICE, DEFAULT_ROLE_DEVICE);
            GetPortsInfo();
            break;
        default:
            printf("%s:%d port param error\n", __func__, __LINE__);
            break;
    }
}

int32_t main(int32_t argc, char *argv[])
{
    if ((argc < 1) || (strcmp(argv[1], "-h") == 0)) {
        PrintHelp();
        return 0;
    }

    if ((!strcmp(argv[1], "-f"))) {
        int32_t mode = stoi(argv[2]);
        FunctionSwitch(g_usbClient, mode);
    } else if (!strcmp(argv[1], "-p")) {
        int32_t mode = stoi(argv[2]);
        PortSwitch(g_usbClient, mode);
    } else {
        printf("param incorrect: please input -h for help\n");
    }
    return 0;
}

