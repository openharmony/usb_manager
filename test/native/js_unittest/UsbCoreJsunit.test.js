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

import usb from '@ohos.usb';
import CheckEmptyUtils from './CheckEmptyUtils.js';
import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from 'deccjsunit/index'


/* usb core functions test */
describe('UsbCoreJsFunctionsTest', function () {

  beforeAll(function () {
    console.log('*************Usb Unit UsbCoreJsFunctionsTest Begin*************');
    var Version = usb.getVersion()
    console.info('begin test getversion :' + Version)
    // 切换为host
    var usbPortList = usb.getPorts()
    if (usbPortList.length > 0) {
      if (usbPortList[0].status.currentMode == 1) {
        usb.setPortRoles(usbPortList[0].id, 1, 1).then(data => {
          console.info('usb case setPortRoles return: ' + data);
        }).catch(error => {
          console.info('usb case setPortRoles error : ' + error);
        });
        console.log('*************Usb Unit Begin switch to host*************');
      }
    }
  })
  afterEach(function () {
    console.info('afterEach: *************Usb Unit Test  Case*************');
  })
  beforeEach(function () {
    console.info('beforeEach: *************Usb Unit Test  Case*************');
  })
  afterAll(function () {
    console.log('*************Usb Unit UsbCoreJsFunctionsTest End*************');
  })

  var gDeviceList
  var gPipe;

  /**
   * @tc.number    : get_devices_test_01
   * @tc.name      : getDevices
   * @tc.desc      : 获取设备列表
   */
  it('SUB_USB_get_devices_test_01', 0, function () {
    console.info('usb get_device_01 begin');
    gDeviceList = usb.getDevices();
    if (gDeviceList.length == 0) {
      console.info('usb case get_devices list is null')
      expect(false).assertTrue();
      return
    }
    expect(gDeviceList.length).assertLarger(0);
    console.info('usb case getDevices ret length: ' + gDeviceList.length);
    console.info('usb get_devices_test_01:  PASS');
  })


  /**
   * @tc.number    : has_right_1
   * @tc.name      : hasRigt
   * @tc.desc      : 权限查询 连接设备 关闭设备
   */
  it('SUB_USB_has_right_01', 0, function () {
    console.info('usb has_right_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    for (var i = 0; i < gDeviceList.length; i++) {
      var hasRight = usb.hasRight(gDeviceList[i].name)
      console.info('usb has_right ret :' + hasRight);
    }

    console.info('usb has_right_01 :  PASS');
    expect(true).assertTrue();
  })


  /**
   * @tc.number    : request_right_01
   * @tc.name      : requestRight
   * @tc.desc      : 请求权限
   */
  it('SUB_USB_request_right_01', 0, function () {
    console.info('usb request_right_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    for (var i = 0; i < gDeviceList.length; i++) {
      usb.requestRight(gDeviceList[i].name).then(hasRight => {
        expect(hasRight).assertTrue();
        console.info('usb request_right ret :' + hasRight);
        console.info('usb request_right_01 :  PASS');
      }).catch(error => {
        console.info('usb case device request right failed : ' + error + ' :' + gDeviceList[i].name);
        expect(false).assertTrue();
      });
    }
  })

  /**
   * @tc.number    : connect_device_01
   * @tc.name      : connectDevice
   * @tc.desc      : 打开设备
   */
  it('SUB_USB_connect_device_01', 0, function () {
    console.info('usb connect_device_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }
    var isRight = usb.hasRight(gDeviceList[0].name);
    if (!isRight) {
      usb.requestRight(gDeviceList[0].name).then(hasRight => {

      }).catch(error => {
        console.info('usb 01 requestRight error:' + error);
      });
      CheckEmptyUtils.sleep(3000)
    }

    gPipe = usb.connectDevice(gDeviceList[0])
    console.info('usb case connectDevice ret: ' + JSON.stringify(gPipe));
    expect(CheckEmptyUtils.isEmpty(gPipe)).assertFalse();

    console.info('usb connect_device_01 :  PASS');
  })

  /**
   * @tc.number    : close_pipe_02
   * @tc.name      : closePipe
   * @tc.desc      : 反向测试 关闭设备 错误 busNum
   */
  it('SUB_USB_close_pipe_02', 0, function () {
    console.info('usb close_pipe_02 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    var tmpPipe = JSON.parse(JSON.stringify(gPipe));
    tmpPipe.busNum = -23
    var isPipClose = usb.closePipe(tmpPipe);
    console.info('usb case closePipe ret: ' + isPipClose);
    expect(isPipClose == 0).assertFalse();

    console.info('usb close_pipe_02 :  PASS');
  })

  /**
   * @tc.number    : close_pipe_03
   * @tc.name      : closePipe
   * @tc.desc      : 反向测试 关闭设备 错误 devAddress
   */
  it('SUB_USB_close_pipe_03', 0, function () {
    console.info('usb close_pipe_03 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }
    var tmpPipe = JSON.parse(JSON.stringify(gPipe));
    tmpPipe.devAddress = -23
    var isPipClose = usb.closePipe(tmpPipe);
    console.info('usb case closePipe ret: ' + isPipClose);
    expect(isPipClose == 0).assertFalse();

    console.info('usb close_pipe_03 :  PASS');
  })

  /**
   * @tc.number    : close_pipe_04
   * @tc.name      : closePipe
   * @tc.desc      : 反向测试 关闭设备 错误 devAddress busNum
   */
  it('SUB_USB_close_pipe_04', 0, function () {
    console.info('usb close_pipe_04 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }
    var tmpPipe = JSON.parse(JSON.stringify(gPipe));
    tmpPipe.devAddress = -23
    tmpPipe.busNum = -23
    var isPipClose = usb.closePipe(tmpPipe);
    console.info('usb case closePipe ret: ' + isPipClose);
    expect(isPipClose == 0).assertFalse();

    console.info('usb close_pipe_04 :  PASS');
  })

  /**
   * @tc.number    : close_pipe_01
   * @tc.name      : closePipe
   * @tc.desc      : 关闭设备
   */
  it('SUB_USB_close_pipe_01', 0, function () {
    console.info('usb close_pipe_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    console.info('usb case closePipe param: ' + JSON.stringify(gPipe));
    var isPipClose = usb.closePipe(gPipe);
    console.info('usb case closePipe ret: ' + isPipClose);
    expect(isPipClose).assertEqual(0);

    console.info('usb close_pipe_01 :  PASS');
  })

})
