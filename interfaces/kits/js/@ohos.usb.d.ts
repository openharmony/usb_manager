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

declare namespace usb {
    /**
     * Obtains the USB service version for test.
     *
     * @return Returns the USB service version.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function getVersion(): string;

    /**
     * Obtains the USB device list.
     *
     * @return Returns the {@link USBDevice} list.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function getDevices(): Array<Readonly<USBDevice>>;

    /**
     * Connects to the USB device based on the device information returned by {@link getDevices()}.
     *
     * @param device USB device on the device list returned by {@link getDevices()}.
     * @return Returns the {@link USBDevicePipe} object for data transfer.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function connectDevice(device: USBDevice): Readonly<USBDevicePipe>;

    /**
     * Checks whether the application has the permission to access the device.
     *
     * @param deviceName Device name defined by {@link USBDevice.name}.
     * @return Returns **true** if the user has the permission to access the device; return **false** otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function hasRight(deviceName: string): boolean;

    /**
     * Requests the temporary permission for a given application to access the USB device.
     *
     * @param deviceName Device name defined by {@link USBDevice.name}.
     * @return Returns **true** if the temporary device access permissions are granted; return **false** otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function requestRight(deviceName: string): Promise<boolean>;

    /**
     * Converts the string descriptor of a given USB function list to a numeric mask combination.
     *
     * @param funcs Descriptor of the supported function list.
     * @return Returns the numeric mask combination of the function list.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function usbFunctionsFromString(funcs: string): number;

    /**
     * Converts the numeric mask combination of a given USB function list to a string descriptor.
     *
     * @param funcs Numeric mask combination of the function list.
     * @return Returns the string descriptor of the supported function list.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function usbFunctionsToString(funcs: FunctionType): string;

    /**
     * Sets the current USB function list in Device mode.
     *
     * @param funcs Numeric mask combination of the supported function list.
      * @return Returns **true** if the setting is successful; returns **false** otherwise.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function setCurrentFunctions(funcs: FunctionType): Promise<boolean>;
    /**
     * Obtains the numeric mask combination for the current USB function list in Device mode.
     *
     * @return Returns the numeric mask combination for the current USB function list in {@link FunctionType}.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function getCurrentFunctions(): FunctionType;

    /* usb port functions begin */
    /**
     * Obtains the {@link USBPort} list.
     *
     * @return Returns the {@link USBPort} list.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function getPorts(): Array<USBPort>;

    /**
     * Gets the mask combination for the supported mode list of the specified {@link USBPort}.
     *
     * @return Returns the mask combination for the supported mode list in {@link PortModeType}.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function getSupportedModes(portId: number): PortModeType;

    /**
     * Sets the role types supported by the specified {@link USBPort}, which can be powerRole (for charging) and dataRole (for data transfer).
     *
     * @param portId Unique ID of the port.
     * @param powerRole Charging role defined by {@link PowerRoleType}.
     * @param dataRole Data role defined by {@link DataRoleType}.
     * @return Returns the supported role type.
     * @systemapi
     * @SysCap SystemCapability.USB.USBManager
     * @since 9
     */
    function setPortRoles(portId: number, powerRole: PowerRoleType, dataRole: DataRoleType): Promise<boolean>;

    /* usb pipe functions begin */
    /**
     * Claims a USB interface.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the bus number and device address.
      * @param iface USB interface defined by {@link USBInterface}, which is used to determine the interface to claim.
     * @param force Optional parameter that determines whether to forcibly claim the USB interface. 
     * @return Returns **0** if the USB interface is successfully claimed; returns an error code otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function claimInterface(pipe: USBDevicePipe, iface: USBInterface, force?: boolean): number;
    /**
     * Releases a USB interface.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the bus number and device address.
      * @param iface USB interface defined by {@link USBInterface}, which is used to determine the interface to release.
     * @return Returns **0** if the USB interface is successfully released; returns an error code otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function releaseInterface(pipe: USBDevicePipe, iface: USBInterface): number;
    /**
     * Sets the device configuration.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the bus number and device address.
     * @param config Device configuration defined by {@link USBConfig}.
     * @return Returns **0** if the device configuration is successfully set; returns an error code otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function setConfiguration(pipe: USBDevicePipe, config: USBConfig): number;
    /**
     * Sets a USB interface.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the bus number and device address.
      * @param iface USB interface defined by {@link USBInterface}, which is used to determine the interface to set.
     * @return Returns **0** if the USB interface is successfully set; return an error code otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function setInterface(pipe: USBDevicePipe, iface: USBInterface): number;
    /**
     * Obtains the raw USB descriptor.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the bus number and device address.
       * @return Returns the raw descriptor data.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function getRawDescriptor(pipe: USBDevicePipe): Uint8Array;
    /**
     * Obtains the file descriptor.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the USB device.
      * @return Returns the file descriptor of the USB device.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function getFileDescriptor(pipe: USBDevicePipe): number;
    /**
      * Performs control transfer.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the USB device.
     * @param contrlparam Control transfer parameters.
     * @param timeout Timeout duration. This parameter is optional. The default value is **0**, indicating no timeout.
     * @return Returns the size of the transmitted or received data block if the control transfer is successful; return **-1** if an exception occurs. 
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function controlTransfer(pipe: USBDevicePipe, contrlparam: USBControlParams, timeout?: number): Promise<number>;

    /**
     * Performs bulk transfer.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the USB device.
     * @param endpoint USB endpoint defined by {@link USBEndpoint}, which is used to determine the USB port for data transfer.
     * @param buffer Buffer for writing or reading data.
     * @param timeout Timeout duration. This parameter is optional. The default value is **0**, indicating no timeout.
     * @return Returns the size of the transmitted or received data block if the control transfer is successful; return **-1** if an exception occurs. 
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function bulkTransfer(pipe: USBDevicePipe, endpoint: USBEndpoint, buffer: Uint8Array,
        timeout?: number): Promise<number>;

    /**
     * Closes a USB device pipe.
     *
     * @param pipe Device pipe defined by {@link USBDevicePipe}, which is used to determine the USB device.
     * @return Returns **0** if the USB device pipe is closed successfully; return an error code otherwise.
     * @SysCap SystemCapability.USB.USBManager
     * @since 8
     */
    function closePipe(pipe: USBDevicePipe): number;

    /**
     * Represents the USB endpoint from which data is sent or received. You can obtain the USB endpoint through {@link USBInterface}.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    interface USBEndpoint {
        /**
        * Endpoint address
        *
        * @since 8
        */
        address: number;

        /**
        * Endpoint attributes
        *
        * @since 8
        */
        attributes: number;

        /**
        * Endpoint interval
        *
        * @since 8
        */
        interval: number;

        /**
        * Maximum size of data packets on the endpoint
        *
        * @since 8
        */
        maxPacketSize: number;

        /**
        * Endpoint direction
        *
        * @since 8
        */
        direction: USBRequestDirection;

        /**
        * Endpoint number
        *
        * @since 8
        */
        number: number;

        /**
        * Endpoint type
        *
        * @since 8
        */
        type: number;

        /**
        * Unique ID defined by {@link USBInterface.id}, which indicates the interface to which the endpoint belongs
        *
        * @since 8
        */
        interfaceId: number;
    }


    /**
     * Represents a USB interface. One USBconfig {@link USBConfig} can contain multiple **USBInterface** instances, each providing a specific function.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    interface USBInterface {
        /**
         * Unique ID of the USB interface
         *
         * @since 8
         */
        id: number;

        /**
         * Interface protocol
         *
         * @since 8
         */
        protocol: number;

        /**
         * Device type
         *
         * @since 8
         */
        clazz: number;

        /**
         * Device subclass
         *
         * @since 8
         */
        subClass: number;

        /**
         * Alternating between descriptors of the same USB interface
         *
         * @since 8
         */
        alternateSetting: number;

        /**
         * Interface name
         *
         * @since 8
         */
        name: string;

        /**
         * {@link USBEndpoint} that belongs to the USB interface
         *
         * @since 8
         */
        endpoints: Array<USBEndpoint>;
    }

    /**
     * USB configuration. One {@link USBDevice} can contain multiple USBConfig instances.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    interface USBConfig {
        /**
         * Unique ID of the USB configuration
         *
         * @since 8
         *
         *
         */
        id: number;

        /**
         * Configuration attributes
         *
         * @since 8
         */
        attributes: number;

        /**
         * Maximum power consumption, in mA
         *
         * @since 8
         */
        maxPower: number;

        /**
         * Configuration name, which can be left empty
         *
         * @since 8
         */
        name: string;

        /**
         * Support for remote wakeup
         *
         * @since 8
         */
        isRemoteWakeup: boolean;

        /**
         * Support for independent power supplies
         *
         * @since 8
         */
        isSelfPowered: boolean;

        /**
         * Supported interface attributes defined by {@link USBInterface}
         *
         * @since 8
         */
        interfaces: Array<USBInterface>;
    }

    /**
     * Represents a USB device.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    interface USBDevice {
        /**
         * Bus address
         *
         * @since 8
         */
        busNum: number;
        /**
         * Device address
         *
         * @since 8
         */
        devAddress: number;
        /**
         * Device SN
         *
         * @since 8
         */
        serial: string;
        /**
         * Device name
         *
         * @since 8
         */
        name: string;
        /**
         * Device manufacturer
         *
         * @since 8
         */
        manufacturerName: string;
        /**
         * Product information
         *
         * @since 8
         */
        productName: string;
        /**
         * Product version
         *
         * @since 8
         */
        version: string;
        /**
         * Vendor ID
         *
         * @since 8
         */
        vendorId: number;
        /**
         * Product ID
         *
         * @since 8
         */
        productId: number;
        /**
         * Device class
         *
         * @since 8
         */
        clazz: number;
        /**
         * Device subclass
         *
         * @since 8
         */
        subClass: number;
        /**
         * Device protocol code
         *
         * @since 8
         */
        protocol: number;
        /**
         * Device configuration descriptor information defined by {@link USBConfig}
         *
         * @since 8
         */
        configs: Array<USBConfig>;
    }

    /**
     * Represents a USB device pipe, which is used to determine the USB device.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    interface USBDevicePipe {
        /**
         * Bus address.
         *
         * @since 8
         */
        busNum: number;
        /**
         * Device address
         *
         * @since 8
         */
        devAddress: number;
    }

    /**
     * Enumerates power role types.
     *
     * @syscap SystemCapability.USB.USBManager
     *
     * @since 9
     */
    export enum PowerRoleType {
        /**
         * None
         *
         * @since 9
         */
        NONE = 0,
        /**
         * External power supply
         *
         * @since 9
         */
        SOURCE = 1,
        /**
         * Internal power supply
         *
         * @since 9
         */
        SINK = 2
    }

    /**
     * Enumerates data role types.
     *
     * @syscap SystemCapability.USB.USBManager
     *
     * @since 9
     */
    export enum DataRoleType {
        /**
         * None
         *
         * @since 9
         */
        NONE = 0,
        /**
         * Host mode
         *
         * @since 9
         */
        HOST = 1,
        /**
         * Device mode
         *
         * @since 9
         */
        DEVICE = 2
    }

    /**
     * Enumerates port mode types
     *
     * @syscap SystemCapability.USB.USBManager
     *
     * @since 9
     */
    export enum PortModeType {
        /**
         * None
         *
         * @since 9
         */
        NONE = 0,
        /**
         * Upstream facing port, which functions as the sink of power supply
         *
         * @since 9
         */
        UFP = 1,
        /**
         * Downstream facing port, which functions as the source of power supply
         *
         * @since 9
         */
        DFP = 2,
        /**
         * Dynamic reconfiguration port (DRP), which can function as the DFP (host) or UFP (device). It is not supported currently.
         *
         * @since 9
         */
        DRP = 3,
        /**
         * Not supported currently
         *
         * @since 9
         */
        NUM_MODES = 4
    }

    /**
     * Enumerates USB device port roles.
     *
     * @syscap SystemCapability.USB.USBManager
     *
     * @since 9
     */
    interface USBPortStatus {
        /**
         * USB mode
         *
         * @since 9
         */
        currentMode: number;

        /**
         * Power role
         *
         * @since 9
         */
        currentPowerRole: number;

        /**
         * Data role
         *
         * @since 9
         */
        currentDataRole: number;
    }

    /**
     * Represents a USB device port.
     *
     * @syscap SystemCapability.USB.USBManager
     *
     * @since 9
     */
    interface USBPort {
        /**
         * Unique ID of the USB port
         *
         * @since 9
         */
        id: number;

        /**
         * Mask combination for the supported mode list of the USB port
         *
         * @since 9
         */
        supportedModes: PortModeType;

        /**
         * USB port role defined by {@link USBPortStatus}
         *
         * @since 9
         */
        status: USBPortStatus;
    }

    /**
    * Represents control transfer parameters.
    *
    * @syscap SystemCapability.USB.USBManager
    * @since 8
    */
    interface USBControlParams {
        /**
         * Request type
         *
         * @since 8
         */
        request: number;
        /**
         * Request target type
         *
         * @since 8
         */
        target: USBRequestTargetType;
        /**
         * Control request type
         *
         * @since 8
         */
        reqType: USBControlRequestType;
        /**
         * Request parameter value
         *
         * @since 8
           */
        value: number;
        /**
         * Index of the parameter value
         *
         * @since 8
         */
        index: number;
        /**
         * Data written to or read from the buffer
         * @since 8
         */
        data: Uint8Array;
    }

    /**
     * Enumerates USB request target types.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    export enum USBRequestTargetType {
        /**
         * USB device
         *
         * @since 8
         */
        USB_REQUEST_TARGET_DEVICE = 0,
        /**
         * USB interface
         *
         * @since 8
         */
        USB_REQUEST_TARGET_INTERFACE = 1,
        /**
         * Endpoint
         *
         * @since 8
         */
        USB_REQUEST_TARGET_ENDPOINT = 2,
        /**
         * Others
         *
         * @since 8
         */
        USB_REQUEST_TARGET_OTHER = 3
    }

    /**
     * Enumerates control request types.
     * 
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    export enum USBControlRequestType {
        /**
         * Standard
         *
         * @since 8
         */
        USB_REQUEST_TYPE_STANDARD = 0,
        /**
         * Class
         *
         * @since 8
         */
        USB_REQUEST_TYPE_CLASS = 1,
        /**
         * Vendor
         *
         * @since 8
         */
        USB_REQUEST_TYPE_VENDOR = 2
    }

    /**
     * Enumerates request directions.
     * 
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     */
    export enum USBRequestDirection {
        /**
         * Request for writing data from the host to the device
         *
         * @since 8
         */
        USB_REQUEST_DIR_TO_DEVICE = 0,
        /**
         * Request for reading data from the device to the host
         *
         * @since 8
         */
        USB_REQUEST_DIR_FROM_DEVICE = 0x80
    }

    /**
     * Enumerates function modes.
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 9
     */
    export enum FunctionType {
        /**
         * None
         *
         * @since 9
         */
        NONE = 0,
        /**
         * Serial port device
         *
         * @since 9
         */
        ACM = 1,
        /**
         * Ethernet port device
         *
         * @since 9
         */
        ECM = 2,
        /**
         * HDC device
         *
         * @since 9
         */
        HDC = 4,
        /**
         * MTP device
         *
         * @since 9
         */
        MTP = 8,
        /**
         * PTP device
         *
         * @since 9
         */
        PTP = 16,
        /**
         * RNDIS device
         *
         * @since 9
         */
        RNDIS = 32,
        /**
         * MIDI device
         *
         * @since 9
         */
        MIDI = 64,
        /**
         * Audio source device
         *
         * @since 9
         */
        AUDIO_SOURCE = 128,
        /**
         * NCM device
         *
         * @since 9
         */
        NCM = 256
    }

}

export default usb;
