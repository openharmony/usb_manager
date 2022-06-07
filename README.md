# USB Manager `<a name="EN-US_TOPIC_0000001124094823"></a>`

- [Introduction](#section11660541593)
- [Directory Structure](#section19472752217)
- [Available APIs](#section19472752218)
- [Development Example](#section19472752219)
- [Repositories Involved](#section63151229062)

## Introduction`<a name="section11660541593"></a>`

The following figure shows the USB service architecture.

**Figure 1**  USB service architecture`<a name="fig15658513184019"></a>`
![](figures/usb-manager-architecture.png "USB_service_architecture")

The architecture logically consists of three layers:

1. USB API: provides USB APIs that implement various basic functions, for example, query of the USB device list, USB device plug notification, USB host and device mode switching, bulk transfer, control transfer, right management, and function switching in device mode.
2. USB Service: interacts with the HAL layer to receive, parse, and distribute data, manages foreground and background policies, and performs USB device management and right control.
3. USB HAL: provides driver capability APIs that can be directly called in user mode. The APIs are classified into the DDK initialization class, interface operation class, and request operation class by function. They can be used to perform DDK initialization, bind/release and open/close an interface, allocate/release a request, and implement isochronous or non-isochronous transfer. The USB HAL code is stored in [drivers\_peripheral](https://gitee.com/openharmony/drivers_peripheral/blob/master/README.md).

## Directory Structure`<a name="section19472752217"></a>`

```
base/usb/usb_manager
├── interfaces                  # APIs
│   ├── innerkits                  # Internal APIs
│   └── kits                       # External APIs
├── sa_profile                  # SA profile
└── services                    # Services
│   ├── native                     # Native APIs
│   └── zidl                       # zidl APIs
└── test                        # Test cases
└── utils                       # Utilities
```

## Available APIs`<a name="section19472752218"></a>`

### [External APIs](https://gitee.com/openharmony/usb_manager/blob/master/interfaces/kits/js/@ohos.usb.d.ts)

### Internal APIs

#### Host APIs

| **API**                                                                                                        | **Description**                                                                                                                                                                                                                                                                                                                               |
| -------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| int32_t OpenDevice(const UsbDevice &device, USBDevicePipe &pip);                                                     | Opens a USB device to set up a connection.                                                                                                                                                                                                                                                                                                          |
| bool Close(const USBDevicePipe &pip);                                                                                | Closes a USB device to release all system resources related to the device.                                                                                                                                                                                                                                                                          |
| int32_t GetDevices(std::vector &deviceList);                                                                         | Obtains the USB device list.                                                                                                                                                                                                                                                                                                                        |
| int32_t SetConfiguration(USBDevicePipe &pip, const USBConfig &config);                                               | Sets the current configuration of the USB device.                                                                                                                                                                                                                                                                                                   |
| int32_t ClaimInterface(USBDevicePipe &pip, const UsbInterface &interface, bool force);                               | Claims a USB interface exclusively. This must be done before data transfer.                                                                                                                                                                                                                                                                         |
| int32_t ReleaseInterface(USBDevicePipe &pip, const UsbInterface &interface);                                         | Releases a USB interface. This is usually done after data transfer.                                                                                                                                                                                                                                                                                 |
| int32_t SetInterface(USBDevicePipe &pipe, const UsbInterface &interface);                                            | Sets the alternate settings for the specified USB interface. This allows you to switch between two interfaces with the same ID but different alternate settings.                                                                                                                                                                                    |
| int32_t BulkTransfer(USBDevicePipe &pip, const USBEndpoint &endpoint, std::vector<uint8_t> &vdata, int32_t timeout); | Writes data on a specified endpoint during bulk transfer.                                                                                                                                                                                                                                                                                           |
| int32_t ControlTransfer(USBDevicePipe &pip, const UsbCtrlTransfer &ctrl, std::vector<uint8_t> &vdata);               | Performs control transfer for endpoint 0 of the device. The data transfer direction is determined by the request type. If the result of**requestType & USB_ENDPOINT_DIR_MASK** is **USB_DIR_OUT**, the endpoint is in the data writing direction; if the result is **USB_DIR_IN**, the endpoint is in the data reading direction. |
| int32_t RequestInitialize(UsbRequest &request);                                                                      | Initializes a request.                                                                                                                                                                                                                                                                                                                              |
| int32_t RequestQueue(UsbRequest &request);                                                                           | Sends or receives requests for isochronous transfer on a specified endpoint. The data transfer direction is determined by the endpoint direction.                                                                                                                                                                                                   |
| int32_t PipeRequestWait(USBDevicePipe &pip, int64_t timeout, UsbRequest &req);                                       | Waits for the operation result of the isochronous transfer request in`<b>`RequestQueue`</b>`.                                                                                                                                                                                                                                                   |
| int32_t RequestAbort(UsbRequest &request);                                                                           | Cancels the data transfer requests to be processed.                                                                                                                                                                                                                                                                                                 |
| int32_t RequestFree(UsbRequest &request);                                                                            | Requests for releasing data.                                                                                                                                                                                                                                                                                                                        |
| bool HasRight(std::string deviceName);                                                                               | Checks whether the application has permission to access the USB device.                                                                                                                                                                                                                                                                             |
| int32_t RequestRight(std::string deviceName);                                                                        | Requests for permission to access the USB device.                                                                                                                                                                                                                                                                                                   |

#### Device APIs

| **API**                                      | **Description**                                                                      |
| -------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| int32_t GetCurrentFunctions(int32_t &funcs);      | Obtains the list of functions (represented by bit field) supported by the current device.  |
| int32_t SetCurrentFunctions(int32_t funcs);       | Sets the list of functions (represented by bit field) supported by the current device.     |
| int32_t UsbFunctionsFromString(std::string funcs); | Converts the string descriptor of a given USB function list to a numeric mask combination. |
| std::string UsbFunctionsToString(int32_t funcs);   | Converts the numeric mask combination of a given USB function list to a string descriptor. |

#### Port APIs

| **API**                                                             | **Description**                                                     |
| ------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| int32_t GetPorts(std::vector &usbPorts);                                  | Obtains the port list.                                                    |
| int32_t SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole); | Sets the port role.                                                       |
| int32_t GetSupportedModes(int32_t portId, int32_t &supportedModes);       | Obtains the mask combination for the supported mode list of a given port. |

## Development Example`<a name="section19472752219"></a>`

The following provides some examples on how to use major APIs. For detailed service code, see [js_unittest](test/native/js_unittest).

#### Host Development

In this example, the USB device serves as the host and connects to the device to implement data transfer.

1. Obtain the USB device list.

```JS
// Import the USB API package.
import usb from '@ohos.usb';
// Obtain the USB device list.
var deviceList = usb.getDevices();
```

2. Obtain the device operation permissions.

```JS
// device name
var deviceName = deviceList[0].name;
// Request for permission to operate a specified device.
usb.requestRight(deviceName).then(hasRight => {
	console.info("usb device request right result: " + hasRight);
}).catch(error => {
	console.info("usb device request right failed : " + error);
});
```

3. Open the USB device.

```JS
// Open the USB device, and obtain the device pipe for data transfer.
var pipe = usb.connectDevice(deviceList[0]);
// Claim the USB interface.
usb.claimInterface(pipe, interface, true); // Among the input arguments, interface refers to the one to be operated on the USB device.
```

4. Perform data transfer.

```JS
// Read data. Select the corresponding RX endpoint (endpoint.direction == 0x80) from deviceList for data transfer. The data data to be transferred is encapsulated in dataUint8Array.
usb.bulkTransfer(pipe, inEndpoint, dataUint8Array, 15000).then(dataLength => {
if (dataLength >= 0) {
	console.info("usb readData result Length : " + dataLength);
	var resultStr = this.ab2str(dataUint8Array); // Convert uint8 data into a string.
	console.info("usb readData buffer : " + resultStr);
} else {
	console.info("usb readData failed : " + dataLength);
}
}).catch(error => {
console.info("usb readData error : " + JSON.stringify(error));
});
// Send data. Select the corresponding TX endpoint (endpoint.direction == 0) from deviceList for data transfer. 
usb.bulkTransfer(this.pip, this.outEndpoint, dataUint8Array, 15000).then(dataLength => {
	if (dataLength >= 0) {
		console.info("usb writeData result write length : " + dataLength);
	} else {
		console.info("writeData failed");
	}
}).catch(error => {
	console.info("usb writeData error : " + JSON.stringify(error));
});
```

5. Release the USB interface, and close the USB device.

```JS
usb.releaseInterface(pipe, interface);
usb.closePipe(pipe);
```

#### Device Development

In this example, the USB device is used as the device to set the ACM, ECM, and HDC functions.

1. Set the USB functions.

```JS
usb.setCurrentFunctions(funType).then(data => {
	console.info("usb setCurrentFunctions : " + data);
}).catch(error => {
	console.info("usb setCurrentFunctions error : " + error);
});
```

## Repositories Involved`<a name="section63151229062"></a>`

[Drive Subsystem](https://gitee.com/openharmony/docs/blob/master/en/readme/driver-subsystem.md)

[drivers\_peripheral](https://gitee.com/openharmony/drivers_peripheral/blob/master/README.md)

[drivers\_framework](https://gitee.com/openharmony/drivers_framework/blob/master/README.md)

[drivers\_adapter](https://gitee.com/openharmony/drivers_adapter/blob/master/README.md)

[drivers\_adapter\_khdf\_linux](https://gitee.com/openharmony/drivers_adapter_khdf_linux/blob/master/README.md)
