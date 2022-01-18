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

#ifndef USB_REQUEST_H
#define USB_REQUEST_H

#include "iusb_srv.h"
#include "usb_device_pipe.h"
#include "usb_endpoint.h"

namespace OHOS {
namespace USB {
class UsbRequest {
public:
    UsbRequest();
    ~UsbRequest() {}
    int32_t Initialize(const USBDevicePipe &pipe, const USBEndpoint &endpoint);
    int32_t Queue();
    int32_t Free();
    int32_t Abort();
    void SetPipe(const USBDevicePipe &pipe)
    {
        this->pipe = pipe;
    }
    void SetEndpoint(const USBEndpoint &endpoint)
    {
        this->endpoint = endpoint;
    }
    void SetClientData(std::vector<uint8_t> bufferData)
    {
        clientData = bufferData;
    }
    void SetReqData(std::vector<uint8_t> bufferData)
    {
        reqData = bufferData;
    }
    std::vector<uint8_t> &GetClientData()
    {
        return clientData;
    }
    std::vector<uint8_t> &GetReqData()
    {
        return reqData;
    }
    const USBDevicePipe &GetPipe() const
    {
        return pipe;
    }
    const USBEndpoint &GetEndpoint() const
    {
        return endpoint;
    }

public:
    USBDevicePipe pipe;
    USBEndpoint endpoint;
    std::vector<uint8_t> clientData;
    std::vector<uint8_t> reqData;

private:
    uint8_t *usbClient;
};
} // namespace USB
} // namespace OHOS

#endif // USB_REQUEST_H
