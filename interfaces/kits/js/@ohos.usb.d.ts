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

declare namespace usb {
    /**
     * 获取USB服务版本号，测试用
     *
     * @return USB服务当前的版本号.
     * @since 8
     */
    function getVersion(): string;

    /* usb core functions begin  */
    /**
     * 获取USB设备列表
     *
     * @return 设备信息{@link USBDevice}列表.
     * @since 8
     */
    function getDevices(): Array<Readonly<USBDevice>>;

    /**
     * 根据{@link getDevices()}返回的设备信息打开USB设备
     *
     * @param device 设备信息，{@link getDevices()}返回的列表中的一个设备信息
     * @return 指定的传输通道 {@link USBDevicePipe} 对象.
     * @since 8
     */
    function connectDevice(device: USBDevice): Readonly<USBDevicePipe>;

    /**
     * 判断是否有权访问设备
     *
     * @param deviceName 设备名称，{@link USBDevice.name}
     * @return true 有权限， false 没有权限
     * @since 8
     */
    function hasRight(deviceName: string): boolean;

    /**
     * 请求给定软件包的临时权限以访问设备
     *
     * @param deviceName 设备名称，{@link USBDevice.name}
     * @return true 请求权限成功， false 请求权限失败
     * @since 8
     */
    function requestRight(deviceName: string): Promise<boolean>;

    /**
     * 将给定的功能列表描述字符串转换为功能列表的数字组合掩码
     *
     * @param funcs 支持的功能列表描述符
     * @return 功能列表的数字组合掩码
     * @systemapi
     * @since 8
     */
    function usbFunctionsFromString(funcs: string): number;

    /**
     * 将给定的功能列表的数字组合掩码转换为功能列表描述字符串
     *
     * @param funcs 支持的功能列表的数字组合掩码
     * @return 支持的功能列表描述字符串
     * @systemapi
     * @since 8
     */
    function usbFunctionsToString(funcs: FunctionType): string;

    /**
     * 在设备模式下设置当前的USB功能列表
     *
     * @param funcs 设置支持的功能列表的数字组合掩码{@link FunctionType}
     * @return true 设置成功， false 设置失败
     * @systemapi
     * @since 8
     */
    function setCurrentFunctions(funcs: FunctionType): Promise<boolean>;
    /**
     * 获取设备模式下的当前USB功能列表的数字组合掩码
     *
     * @return 支持的功能列表的数字组合掩码{@link FunctionType}
     * @systemapi
     * @since 8
     */
    function getCurrentFunctions(): FunctionType;

    /* usb port functions begin */
    /**
     * 获取物理USB端口描述信息{@link USBPort}列表
     *
     * @return {@link USBPort}列表
     * @systemapi
     * @since 8
     */
    function getPorts(): Array<USBPort>;

    /**
     * 获取指定的端口{@link USBPort}支持的模式列表的组合掩码
     *
     * @return 支持的模式列表的组合掩码{@link PortModeType}
     * @systemapi
     * @since 8
     */
    function getSupportedModes(portId: number): PortModeType;

    /**
     * 设置指定的端口{@link USBPort}支持的角色模式，包含充电角色、数据传输角色
     *
     * @param portId 端口的唯一标识
     * @param powerRole 充电的角色{@link PowerRoleType}
     * @param dataRole 数据传输的角色{@link DataRoleType}
     * @return 支持的模式
     * @systemapi
     * @since 8
     */
    function setPortRoles(portId: number, powerRole: PowerRoleType, dataRole: DataRoleType): Promise<boolean>;

    /* usb pipe functions begin */
    /**
     * 获取接口
     *
     * @param pipe 用于确定总线号和设备地址 {@link USBDevicePipe}.
     * @param iface 用于确定需要获取接口 {@link USBInterface}.
     * @param force 是否强制获取
     * @return 成功：0；失败：错误码
     * @since 8
     */
    function claimInterface(pipe: USBDevicePipe, iface: USBInterface, force?: boolean): number;
    /**
     * 释放接口
     *
     * @param pipe 用于确定总线号和设备地址 {@link USBDevicePipe}.
     * @param iface 用于确定需要释放接口 {@link USBInterface}.
     * @return 成功：0；失败：错误码
     * @since 8
     */
    function releaseInterface(pipe: USBDevicePipe, iface: USBInterface): number;
    /**
     * 设置设备配置
     *
     * @param pipe 用于确定总线号和设备地址 {@link USBDevicePipe}.
     * @param config 用于确定需要设置配置 {@link USBConfig}.
     * @return 成功：0；失败：错误码
     * @since 8
     */
    function setConfiguration(pipe: USBDevicePipe, config: USBConfig): number;
    /**
     * 设置设备接口
     *
     * @param pipe 用于确定总线号和设备地址 {@link USBDevicePipe}.
     * @param iface 用于确定需要设置接口 {@link USBInterface}.
     * @return 成功：0；失败：错误码
     * @since 8
     */
    function setInterface(pipe: USBDevicePipe, iface: USBInterface): number;
    /**
     * 获取原始的USB描述符
     *
     * @param pipe 用于确定总线号和设备地址 {@link USBDevicePipe}.
     * @return 返回获取的原始数据
     * @since 8
     */
    function getRawDescriptor(pipe: USBDevicePipe): Uint8Array;
    /**
     * 获取文件描述符
     *
     * @param pipe 用于确定设备 {@link USBDevicePipe}.
     * @return 返回设备对应的文件描述符
     * @since 8
     */
    function getFileDescriptor(pipe: USBDevicePipe): number;
    /**
     * 控制传输
     *
     * @param pipe 用于确定设备 {@link USBDevicePipe}.
     * @param contrlparam 控制传输参数
     * @param timeout 超时时间，可选参数，默认为0不超时
     * @return 传输或接收到的数据块大小，异常返回-1
     * @since 8
     */
    function controlTransfer(pipe: USBDevicePipe, contrlparam: USBControlParams, timeout?: number): Promise<number>;

    /**
     * 批量传输
     *
     * @param pipe 用于确定设备 {@link USBDevicePipe}.
     * @param endpoint 用于确定传输的端口 {@link USBEndpoint}.
     * @param buffer 用于写入或读取的缓冲区
     * @param timeout 超时时间，可选参数，默认为0不超时
     * @return 传输或接收到的数据块大小，异常返回-1
     * @since 8
     */
    function bulkTransfer(pipe: USBDevicePipe, endpoint: USBEndpoint, buffer: Uint8Array,
        timeout?: number): Promise<number>;

    /**
     * 关闭USBDevicePipe
     *
     * @param pipe 用于确定pipe {@link USBDevicePipe}.
     * @return 成功：0；失败：错误码
     * @since 8
     */
    function closePipe(pipe: USBDevicePipe): number;

    /**
     * 通过USB发送和接收数据的端口。通过{@link USBInterface}获取。
     *
     * @since 8
     */
    interface USBEndpoint {
        /**
        * 端点地址
        *
        * @since 8
        */
        address: number;

        /**
        * 端点属性
        *
        * @since 8
        */
        attributes: number;

        /**
        * 端点间隔
        *
        * @since 8
        */
        interval: number;

        /**
        * 端点最大数据包大小
        *
        * @since 8
        */
        maxPacketSize: number;

        /**
        * 端点的方向。
        *
        * @since 8
        */
        direction: USBRequestDirection;

        /**
        * 端点号
        *
        * @since 8
        */
        number: number;

        /**
        * 端点类型
        *
        * @since 8
        */
        type: number;

        /**
        * 端点所属的接口的唯一标识{@link USBInterface.id}
        *
        * @since 8
        */
        interfaceId: number;
    }


    /**
     * 一个{@link USBConfig}中可以含有多个interface，每个interface提供一个功能。
     *
     * @since 8
     */
    interface USBInterface {
        /**
         * 接口的唯一标识
         *
         * @since 8
         */
        id: number;

        /**
         * 接口的协议
         *
         * @since 8
         */
        protocol: number;

        /**
         * 设备类型
         *
         * @since 8
         */
        clazz: number;

        /**
         * 设备子类
         *
         * @since 8
         */
        subClass: number;

        /**
         *在同一个接口中的多个描述符中进行切换设置
         *
         * @since 8
         */
        alternateSetting: number;

        /**
         * 接口名称
         *
         * @since 8
         */
        name: string;

        /**
         * 当前接口所包含的端点{@link USBEndpoint}
         *
         * @since 8
         */
        endpoints: Array<USBEndpoint>;
    }

    /**
     * USB配置，一个{@link USBDevice}中可以含有多个配置。
     *
     * @since 8
     */
    interface USBConfig {
        /**
         * 配置的唯一标识
         *
         * @since 8
         *
         *
         */
        id: number;

        /**
         * 配置的属性
         *
         * @since 8
         */
        attributes: number;

        /**
         * 最大功耗，以毫安为单位
         *
         * @since 8
         */
        maxPower: number;

        /**
         * 配置的名称，可以为空
         *
         * @since 8
         */
        name: string;

        /**
         * 检查当前配置是否支持远程唤醒
         *
         * @since 8
         */
        isRemoteWakeup: boolean;

        /**
         * 检查当前配置是否支持独立电源
         *
         * @since 8
         */
        isSelfPowered: boolean;

        /**
         * 配置支持的接口属性{@link USBInterface}
         *
         * @since 8
         */
        interfaces: Array<USBInterface>;
    }

    /**
     * USB设备信息。
     *
     * @since 8
     */
    interface USBDevice {
        /**
         * 总线地址
         *
         * @since 8
         */
        busNum: number;
        /**
         * 设备地址
         *
         * @since 8
         */
        devAddress: number;
        /**
         * 序列号
         *
         * @since 8
         */
        serial: string;
        /**
         * 设备名字
         *
         * @since 8
         */
        name: string;
        /**
         * 产商信息
         *
         * @since 8
         */
        manufacturerName: string;
        /**
         * 产品信息
         *
         * @since 8
         */
        productName: string;
        /**
         * 版本
         *
         * @since 8
         */
        version: string;
        /**
         * 厂商ID
         *
         * @since 8
         */
        vendorId: number;
        /**
         * 产品ID
         *
         * @since 8
         */
        productId: number;
        /**
         * 设备类
         *
         * @since 8
         */
        clazz: number;
        /**
         * 设备子类
         *
         * @since 8
         */
        subClass: number;
        /**
         * 设备协议码
         *
         * @since 8
         */
        protocol: number;
        /**
         * 设备配置描述符信息 {@link USBConfig}.
         *
         * @since 8
         */
        configs: Array<USBConfig>;
    }

    /**
     * USB设备消息控制通道，用于确定设备
     *
     * @since 8
     */
    interface USBDevicePipe {
        /**
         * 总线地址
         *
         * @since 8
         */
        busNum: number;
        /**
         * 设备地址
         *
         * @since 8
         */
        devAddress: number;
    }

    /**
     * 电源角色类型
     *
     * @since 8
     */
    enum PowerRoleType {
        /**
         * 无
         *
         * @since 8
         */
        NONE = 0,
        /**
         * 外部供电
         *
         * @since 8
         */
        SOURCE,
        /**
         * 内部供电
         *
         * @since 8
         */
        SINK
    }

    /**
     * 数据角色类型
     *
     * @since 8
     */
    enum DataRoleType {
        /**
         * 无
         *
         * @since 8
         */
        NONE = 0,
        /**
         * host模式，主设备模式
         *
         * @since 8
         */
        HOST,
        /**
         * device模式，从设备模式
         *
         * @since 8
         */
        DEVICE
    }

    /**
     * port模式类型
     *
     * @since 8
     */
    enum PortModeType {
        /**
         * 无
         *
         * @since 8
         */
        NONE = 0,
        /**
         * host 数据下行，对外提供电源
         *
         * @since 8
         */
        DFP,
        /**
         * device 数据上行，需要外部供电
         *
         * @since 8
         */
        UFP
    }

    /**
     * USB设备端口角色信息
     *
     * @since 8
     */
    interface USBPortStatus {
        /**
         * 当前的USB模式
         *
         * @since 8
         */
        currentMode: number;

        /**
         * 当前设备充电模式
         *
         * @since 8
         */
        currentPowerRole: number;

        /**
         * 当前设备数据传输模式
         *
         * @since 8
         */
        currentDataRole: number;
    }

    /**
     * USB设备端口信息
     *
     * @since 8
     */
    interface USBPort {
        /**
         * USB端口信息唯一标识
         *
         * @since 8
         */
        id: number;

        /**
         * 当前端口支持的模式列表的组合掩码
         *
         * @since 8
         */
        supportedModes: PortModeType;

        /**
         * 当前端口角色信息 {@link USBPortStatus}
         *
         * @since 8
         */
        status: USBPortStatus;
    }

    /**
    * 控制传输参数
    *
    * @since 8
    */
    interface USBControlParams {
        /**
         * 请求类型
         *
         * @since 8
         */
        request: number;
        /**
         * 请求目标类型
         *
         * @since 8
         */
        target: USBRequestTargetType;
        /**
         * 控制请求类型
         *
         * @since 8
         */
        reqType: USBControlRequestType;
        /**
         * 请求参数
         *
         * @since 8
           */
        value: number;
        /**
         * 根据请求参数value的不同有所不同
         *
         * @since 8
         */
        index: number;
        /**
         * @用于写入或读取的缓冲区
         * @since 8
         */
        data: Uint8Array;
    }

    /**
     * 请求目标类型
     *
     * @since 8
     */
    enum USBRequestTargetType {
        /**
         * 设备
         *
         * @since 8
         */
        USB_REQUEST_TARGET_DEVICE = 0,
        /**
         * 接口
         *
         * @since 8
         */
        USB_REQUEST_TARGET_INTERFACE,
        /**
         * 端点
         *
         * @since 8
         */
        USB_REQUEST_TARGET_ENDPOINT,
        /**
         * 其他
         *
         * @since 8
         */
        USB_REQUEST_TARGET_OTHER
    }

    /**
     * 控制请求类型
     * @since 8
     */
    enum USBControlRequestType {
        /**
         * 标准
         *
         * @since 8
         */
        USB_REQUEST_TYPE_STANDARD = 0,
        /**
         * 类
         *
         * @since 8
         */
        USB_REQUEST_TYPE_CLASS,
        /**
         * 厂商
         *
         * @since 8
         */
        USB_REQUEST_TYPE_VENDOR
    }

    /**
     * 请求方向
     * @since 8
     */
    enum USBRequestDirection {
        /**
         * 写数据，主设备往从设备
         *
         * @since 8
         */
        USB_REQUEST_DIR_TO_DEVICE = 0,
        /**
         * 读数据，从设备往主设备
         *
         * @since 8
         */
        USB_REQUEST_DIR_FROM_DEVICE = 0x80
    }

    /**
     * function 模式
     *
     * @since 8
     */
    enum FunctionType {
        /**
         * 无
         *
         * @since 8
         */
        NONE = 0,
        /**
         * 串口设备
         *
         * @since 8
         */
        ACM = 1,
        /**
         * 网口设备
         *
         * @since 8
         */
        ECM = 2,
        /**
         * HDC设备
         *
         * @since 8
         */
        HDC = 4,
        /**
         * MTP设备
         *
         * @since 8
         */
        MTP = 8,
        /**
         * PTP设备
         *
         * @since 8
         */
        PTP = 16,
        /**
         * RNDIS设备
         *
         * @since 8
         */
        RNDIS = 32,
        /**
         * MIDI设备
         *
         * @since 8
         */
        MIDI = 64,
        /**
         * AUDIO SOURCE设备
         *
         * @since 8
         */
        AUDIO_SOURCE = 128,
        /**
         * NCM设备
         *
         * @since 8
         */
        NCM = 256
    }

}

export default usb;
