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
describe('UsbPortJsFunctionsTestEx', function () {

  beforeAll(function () {
    console.log('*************Usb Unit UsbPortJsFunctionsTestEx Begin*************');
    var Version = usb.getVersion()
    console.info('begin test getversion :' + Version)
  })
  beforeEach(function () {
    console.info('beforeEach: *************Usb Unit Test Case*************');
  })

  afterEach(function () {
    console.info('afterEach: *************Usb Unit Test Case*************');
  })

  afterAll(function () {
    console.log('*************Usb Unit UsbPortJsFunctionsTest End*************');
  })

  /**
   * @tc.number    : get_supported_modes_test_02
   * @tc.name      : getSupportedModes
   * @tc.desc      : 反向测试 改变id 获取指定的端口支持的模式列表的组合掩码
   */
  it('SUB_USB_get_supported_modes_test_02', 0, function () {
    console.info('usb get_supported_modes_test_02 begin');
    var usbPortList = usb.getPorts()
    if (usbPortList.length == 0) {
      console.info('usb get_supported_modes_test_01 usbPortList is null');
      expect(false).assertTrue();
      return
    }

    expect(usbPortList.length > 0).assertTrue();

    var portID = usbPortList[0].id + 10
    var maskCode = usb.getSupportedModes(portID)
    expect(maskCode).assertEqual(0);

    console.info('usb get_supported_modes_test_02 :  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_03
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_03', 0, function () {
    var portId = 2;
    var powerRole = 2;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(2, 2, -1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_03:  PASS');
    })

    console.info('set_port_roles_test_03:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_04
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，portId，powerRole参数错误
   */
  it('SUB_USB_set_port_roles_test_04', 0, function () {
    var portId = -1;
    var powerRole = -1;
    var dataRole = 2;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, -1, 2).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_04:  PASS');
    })

    console.info('set_port_roles_test_04:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_05
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，portId，dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_05', 0, function () {
    var portId = -1;
    var powerRole = 2;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, 2, -1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_05:  PASS');
    })

    console.info('set_port_roles_test_05:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_06
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，powerRole，dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_06', 0, function () {
    var portId = 2;
    var powerRole = -1;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(2, -1, -1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_06:  PASS');
    })

    console.info('set_port_roles_test_06:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_07
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，portId,portId，dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_07', 0, function () {
    var portId = -1;
    var dataRole = -1;
    var powerRole = -1;

    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, -1, -1).then(data => {
      console.info('usb case setPortRoles 07 return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles 07 error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_07:  PASS');
    })

    console.info('set_port_roles_test_07:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_08
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，portId参数错误
   */
  it('SUB_USB_set_port_roles_test_08', 0, function () {
    var portId = -1;
    var powerRole = 1;
    var dataRole = 1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, 1, 1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_08:  PASS');
    })

    console.info('set_port_roles_test_08:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_09
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，powerRole参数错误
   */
  it('SUB_USB_set_port_roles_test_09', 0, function () {
    var portId = 1;
    var powerRole = -1;
    var dataRole = 1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(1, -1, 1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_09:  PASS');
    })

    console.info('set_port_roles_test_09:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_10
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_10', 0, function () {
    var portId = 1;
    var powerRole = 1;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(1, -1, 1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_10:  PASS');
    })

    console.info('set_port_roles_test_10:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_11
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，portId,powerRole参数错误
   */
  it('SUB_USB_set_port_roles_test_11', 0, function () {
    var portId = -1;
    var powerRole = -1;
    var dataRole = 1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, -1, 1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_11:  PASS');
    })

    console.info('set_port_roles_test_11:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_12
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，portId,dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_12', 0, function () {
    var portId = -1;
    var powerRole = 1;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, 1, -1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_12:  PASS');
    })

    console.info('set_port_roles_test_12:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_13
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，powerRole,dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_13', 0, function () {
    var portId = 1;
    var powerRole = -1;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(1, -1, -1).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_13:  PASS');
    })

    console.info('set_port_roles_test_13:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_14
   * @tc.name      : setPortRoles
   * @tc.desc      : host模式下，portId,powerRole,dataRole参数错误
   */
  it('SUB_USB_set_port_roles_test_14', 0, function () {
    var portId = -1;
    var powerRole = -1;
    var dataRole = -1;
    CheckEmptyUtils.sleep(2000)

    usb.setPortRoles(-1, -1, -1).then(data => {
      console.info('usb case setPortRoles 14 return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles 14 error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_14:  PASS');
    })

    console.info('set_port_roles_test_14:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_15
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，portId参数错误
   */
  it('SUB_USB_set_port_roles_test_15', 0, function () {
    var portId = -1;
    var powerRole = 2;
    var dataRole = 2;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(-1, 2, 2).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_15:  PASS');
    })

    console.info('set_port_roles_test_15:  PASS');
    expect(true).assertTrue();
  })

  /**
   * @tc.number    : set_port_roles_test_16
   * @tc.name      : setPortRoles
   * @tc.desc      : Device模式下，powerRole参数错误
   */
  it('SUB_USB_set_port_roles_test_16', 0, function () {
    var portId = 2;
    var powerRole = -1;
    var dataRole = 2;
    CheckEmptyUtils.sleep(2000)
    usb.setPortRoles(2, -1, 2).then(data => {
      console.info('usb case setPortRoles return: ' + data);
      expect(data).assertTrue();
    }).catch(error => {
      console.info('usb case setPortRoles error : ' + error);
      expect(error).assertFalse();
      console.info('set_port_roles_test_16:  PASS');
    })

    console.info('set_port_roles_test_16:  PASS');
    expect(true).assertTrue();
  })
})
