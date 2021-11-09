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
import EventConstants from './EventConstants.js';
import parameter from '@ohos.systemparameter';
import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from 'deccjsunit/index'

/* usb device pipe test */
describe('UsbDevicePipeJsFunctionsTest', function () {
  var gDeviceList
  var gPipe

  beforeAll(function () {
    console.log('*************Usb Unit UsbDevicePipeJsFunctionsTest Begin*************');
    var Version = usb.getVersion()
    console.info('usb unit begin test getversion :' + Version)

    // 切换为host
    var usbPortList = usb.getPorts()
    if (usbPortList.length > 0) {
      if (usbPortList[0].status.currentMode == 2) {
        usb.setPortRoles(usbPortList[0].id, 1, 1).then(data => {
          console.info('usb case setPortRoles return: ' + data);
        }).catch(error => {
          console.info('usb case setPortRoles error : ' + error);
        });
        console.log('*************Usb Unit switch to host Begin*************');
      }
    }

    gDeviceList = usb.getDevices();
    gPipe = usb.connectDevice(gDeviceList[0])
    console.info('usb unit connectDevice gPipe ret : ' + JSON.stringify(gPipe));
  })

  beforeEach(function () {
    console.info('beforeEach: *************Usb Unit Test Case*************');
  })
  afterEach(function () {
    console.info('afterEach: *************Usb Unit Test Case*************');
  })

  afterAll(function () {
    var isPipClose = usb.closePipe(gPipe)
    console.info('usb unit close gPipe ret : ' + isPipClose);
    console.log('*************Usb Unit UsbDevicePipeJsFunctionsTest End*************');
  })

  function findInitPoint(testParam, j) {
    var bfind = false
    for (var k = 0; k < testParam.config.interfaces[j].endpoints.length; k++) {
      var endpoint = testParam.config.interfaces[j].endpoints[k];
      if (endpoint.type == EventConstants.USB_ENDPOINT_XFER_BULK) {
        bfind = true
        if (endpoint.direction == EventConstants.USB_ENDPOINT_DIR_OUT) {
          testParam.outEndpoint = endpoint;
          testParam.maxOutSize = endpoint.maxPacketSize;
        } else if (endpoint.direction == EventConstants.USB_ENDPOINT_DIR_IN) {
          testParam.inEndpoint = endpoint
          testParam.maxInSize = endpoint.maxPacketSize;
        }

      }
    }
    if (bfind) {
      testParam.interface = testParam.config.interfaces[j]
      return true
    }
    return false
  }

  function initPoint(testParam) {
    for (var j = 0; j < testParam.config.interfaces.length; j++) {
      if (testParam.config.interfaces[j].endpoints.length == 0) {
        continue
      }

      if (testParam.config.interfaces[j].clazz != 10 ||
        testParam.config.interfaces[j].subclass != 0 ||
        testParam.config.interfaces[j].protocol != 2) {
        continue;
      }
      if (findInitPoint(testParam, j)) {
        break
      }
    }
  }

  // 预制传输相关参数
  function getTransferTestParam() {
    var testParam = {
      device: null,
      config: null,
      pip: null,
      inEndpoint: null,
      outEndpoint: null,
      interface: null,
      usbRequest: null,
      sendData: '',
      isClaimed: 0,
      maxInSize: 1024,
      maxOutSize: 1024
    }

    console.info('usb case gDeviceList.length: ' + gDeviceList.length);
    for (var i = 0; i < gDeviceList.length; i++) {
      testParam.device = gDeviceList[i]
      testParam.config = testParam.device.configs[0]
      testParam.pip = gPipe
      initPoint(testParam)
    }
    return testParam
  }

  /*
  * @tc.number    : bulk_transfer_test_01
  * @tc.name      : bulkTransfer
  * @tc.desc      : 批量传输 收数据
  */
  it('bulk_transfer_test_01', 0, function () {
    console.info('usb bulk_transfer_test_01 begin');
    var testParam = getTransferTestParam()
    if (testParam.interface == null || testParam.inEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    testParam.isClaimed = usb.releaseInterface(testParam.pip, testParam.interface)
    expect(testParam.isClaimed).assertEqual(0);
    testParam.isClaimed = usb.claimInterface(testParam.pip, testParam.interface, true);
    expect(testParam.isClaimed).assertEqual(0);

    console.info('usb case readData begin');
    var tmpUint8Array = new Uint8Array(testParam.maxInSize);
    usb.bulkTransfer(testParam.pip, testParam.inEndpoint, tmpUint8Array, 5000).then(data => {
      console.info('usb case readData tmpUint8Array buffer : ' + CheckEmptyUtils.ab2str(tmpUint8Array));
      console.info('usb case readData ret: ' + data);
      console.info('usb case bulk_transfer_test_01 :  PASS');
      expect(data >= 0).assertTrue();
    }).catch(error => {
      console.info('usb case readData error : ' + JSON.stringify(error));
      expect(false).assertTrue();
    });
  })

  /*
  * @tc.number    : bulk_transfer_test_02
  * @tc.name      : bulkTransfer
  * @tc.desc      : 批量传输 发数据
  */
  it('bulk_transfer_test_02', 0, function () {
    console.info('usb bulk_transfer_test_02 begin');
    var testParam = getTransferTestParam()
    if (testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    testParam.isClaimed = usb.releaseInterface(testParam.pip, testParam.interface)
    expect(testParam.isClaimed).assertEqual(0);
    testParam.isClaimed = usb.claimInterface(testParam.pip, testParam.interface, true);
    expect(testParam.isClaimed).assertEqual(0);

    testParam.sendData = 'send default';
    try {
      testParam.sendData = parameter.getSync('test_usb', 'default');
      console.log('usb parameter ' + JSON.stringify(testParam.sendData));
    } catch (e) {
      console.log('usb parameter getSync unexpected error: ' + e);
    }

    var tmpUint8Array = CheckEmptyUtils.str2ab(testParam.sendData);
    usb.bulkTransfer(testParam.pip, testParam.outEndpoint, tmpUint8Array, 5000).then(data => {
      console.info('usb case bulk_transfer_test_02 ret: ' + data);
      console.info('usb case bulk_transfer_test_02 send data: ' + testParam.sendData);
      console.info('usb case bulk_transfer_test_02 :  PASS');
      expect(true).assertTrue();
    }).catch(error => {
      console.info('usb write error : ' + JSON.stringify(error));
      expect(false).assertTrue();
    });

  })

  /*
  * @tc.number    : claim_interface_test_01
  * @tc.name      : claimInterface
  * @tc.desc      : 获取接口 并释放
  */
  it('claim_interface_test_01', 0, function () {
    console.info('usb claim_interface_test_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb 01 case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    if (gDeviceList[0].configs.length == 0) {
      console.info('usb 01 case current device.configs.length = 0');
      expect(false).assertTrue();
      return
    }

    for (var j = 0; j < gDeviceList[0].configs.length; j++) {
      if (gDeviceList[0].configs[j].interfaces.length == 0) {
        console.info('usb case current device.configs.interfaces.length = 0');
      }
      for (var k = 0; k < gDeviceList[0].configs[j].interfaces.length; k++) {
        var isClaim = usb.claimInterface(gPipe, gDeviceList[0].configs[j].interfaces[k], true)
        console.info('usb case claimInterface function return: ' + isClaim);
        expect(isClaim).assertEqual(0);
        if (isClaim == 0) {
          isClaim = usb.releaseInterface(gPipe, gDeviceList[0].configs[j].interfaces[k])
          console.info('usb case releaseInterface function return: ' + isClaim);
          expect(isClaim).assertEqual(0);
        }
      }
    }

    console.info('usb claim_interface_test_01 :  PASS');
    expect(true).assertTrue();
  })

  function getTransferParam(iCmd, iReqType, iValue, iIndex) {
    var tmpUint8Array = new Uint8Array(512);
    var requestCmd = iCmd
    var requestType = iReqType
    var value = iValue;
    var index = iIndex;
    var controlParam = {
      request: requestCmd,
      reqType: requestType,
      value: value,
      index: index,
      data: tmpUint8Array
    }
    return controlParam
  }

  /*
  * @tc.number    : set_configuration_test_01
  * @tc.name      : setConfiguration
  * @tc.desc      : 设置设备接口
  */
  it('set_configuration_test_01', 0, function () {
    console.info('usb set_configuration_test_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    for (var j = 0; j < gDeviceList[0].configs.length; j++) {
      var ret = usb.setConfiguration(gPipe, gDeviceList[0].configs[j])
      console.info('usb case setConfiguration return : ' + ret);
      expect(ret).assertEqual(0);
    }

    console.info('usb set_configuration_test_01 :  PASS');
    expect(true).assertTrue();
  })

  /*
  * @tc.number    : set_configuration_test_02
  * @tc.name      : setConfiguration
  * @tc.desc      : 反向测试 设置设备接口
  */
  it('set_configuration_test_02', 0, function () {
    console.info('usb set_configuration_test_02 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    for (var j = 0; j < gDeviceList[0].configs.length; j++) {
      var config = JSON.parse(JSON.stringify(gDeviceList[0].configs[j]));
      config.id = 255
      var ret = usb.setConfiguration(gPipe, config)
      console.info('usb case setConfiguration return : ' + ret);
      expect(ret).assertLess(0);
    }

    console.info('usb set_configuration_test_02 :  PASS');
  })

  /*
  * @tc.number    : set_interface_test_01
  * @tc.name      : setInterface
  * @tc.desc      : 设置设备接口
  */
  it('set_interface_test_01', 0, function () {
    console.info('usb set_interface_test_01 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue();
      return
    }

    for (var j = 0; j < gDeviceList[0].configs.length; j++) {
      var isClaim = usb.claimInterface(gPipe, gDeviceList[0].configs[j].interfaces[0], true)
      expect(isClaim).assertEqual(0)
      if (isClaim == 0) {
        var ret = usb.setInterface(gPipe, gDeviceList[0].configs[j].interfaces[0])
        expect(ret).assertEqual(0);
        console.info('usb case setInterface return : ' + ret);
      }
    }

    console.info('usb set_interface_test_01 :  PASS');
    expect(true).assertTrue();
  })

  /*
  * @tc.number    : set_interface_test_02
  * @tc.name      : setInterface
  * @tc.desc      : 反向测试 设置设备接口
  */
  it('set_interface_test_02', 0, function () {
    console.info('usb set_interface_test_02 begin');
    if (gDeviceList.length == 0) {
      console.info('usb case get_device_list is null')
      expect(false).assertTrue()
      return
    }

    for (var j = 0; j < gDeviceList[0].configs.length; j++) {
      var isClaim = usb.claimInterface(gPipe, gDeviceList[0].configs[j].interfaces[0], true)
      expect(isClaim).assertEqual(0)
      if (isClaim == 0) {
        var tmpInterface = JSON.parse(JSON.stringify(gDeviceList[0].configs[j].interfaces[0]));
        tmpInterface.id = 234
        var ret = usb.setInterface(gPipe, tmpInterface)
        expect(ret).assertLess(0)
        console.info('usb case setInterface return : ' + ret)
      }
    }

    console.info('usb set_interface_test_02 :  PASS');
    expect(true).assertTrue();
  })

  function callControlTransfer(pip, controlParam, timeout, caseName) {
    usb.controlTransfer(pip, controlParam, timeout).then(data => {
      console.info('usb controlTransfer ret data : ' + data + ' ' + caseName);
      console.info('usb controlTransfer controlParam.data buffer : ' + controlParam.data + ' ' + caseName);
      console.info('usb' + caseName + ':  PASS');
      expect(true).assertTrue();
    }).catch(error => {
      console.info('usb controlTransfer error : ' + JSON.stringify(error));
      console.info('usb' + caseName + ':  PASS');
      expect(false).assertTrue();
    });
  }

  /*
  * @tc.number    : control_transfer_test_01
  * @tc.name      : controlTransfer
  * @tc.desc      : 控制传输 GetDescriptor: cmd 6 reqType 128 value 512 index 0
  */
  it('control_transfer_test_01', 0, function () {
    console.info('usb control_transfer_test_01 begin');
    var testParam = getTransferTestParam()
    if (testParam.inEndpoint == null || testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    var timeout = 5000;
    var controlParam = getTransferParam(6, ((1 << 7) | (0 << 5) | (0 & 0x1f)), (2 << 8), 0)
    callControlTransfer(testParam.pip, controlParam, timeout, 'control_transfer_test_01 GetDescriptor')
  })

  /*
  * @tc.number    : control_transfer_test_02
  * @tc.name      : controlTransfer
  * @tc.desc      : 控制传输 GetStatus: cmd 0 reqType 128 value 0 index 0
  */
  it('control_transfer_test_02', 0, function () {
    console.info('usb control_transfer_test_02 begin');
    var testParam = getTransferTestParam()
    if (testParam.inEndpoint == null || testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    var timeout = 5000;
    var controlParam = getTransferParam(0, 128, 0, 0)
    callControlTransfer(testParam.pip, controlParam, timeout, 'control_transfer_test_02 GetStatus')
  })

  /*
  * @tc.number    : control_transfer_test_03
  * @tc.name      : controlTransfer
  * @tc.desc      : 控制传输 GetConfiguration: cmd 8 reqType 128 value 0 index 0
  */
  it('control_transfer_test_03', 0, function () {
    console.info('usb control_transfer_test_03 begin');
    var testParam = getTransferTestParam()
    if (testParam.inEndpoint == null || testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    var timeout = 5000;
    var controlParam = getTransferParam(8, 128, 0, 0)
    callControlTransfer(testParam.pip, controlParam, timeout, 'control_transfer_test_03 GetConfiguration')
  })

  /*
  * @tc.number    : control_transfer_test_04
  * @tc.name      : controlTransfer
  * @tc.desc      : 控制传输 GetInterface: cmd 10 reqType 129 value 0 index 1
  */
  it('control_transfer_test_04', 0, function () {
    console.info('usb control_transfer_test_04 begin');
    var testParam = getTransferTestParam()
    if (testParam.inEndpoint == null || testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    var timeout = 5000;
    var controlParam = getTransferParam(10, 129, 0, 1)
    callControlTransfer(testParam.pip, controlParam, timeout, 'control_transfer_test_04 GetInterface')
  })

  /*
  * @tc.number    : control_transfer_test_05
  * @tc.name      : controlTransfer
  * @tc.desc      : 控制传输 ClearFeature: cmd 1 reqType 0 value 0 index 0
  */
  it('control_transfer_test_05', 0, function () {
    console.info('usb control_transfer_test_05 begin');
    var testParam = getTransferTestParam()
    if (testParam.inEndpoint == null || testParam.interface == null || testParam.outEndpoint == null) {
      expect(false).assertTrue();
      return
    }

    var timeout = 5000;
    var controlParam = getTransferParam(1, 0, 0, 0)
    callControlTransfer(testParam.pip, controlParam, timeout, 'control_transfer_test_05 ClearFeature')
  })

})
