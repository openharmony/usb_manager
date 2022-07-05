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

import router from '@ohos.router'

export default {
    data: {
        prompt: '',
        allowButton: '',
        notAllowButton: '',
    },
    onInit() {
        this.prompt = this.$t("message.allow") + router.getParams()['bundleName'] + this.$t("message.use") +
            router.getParams()['deviceName'] + '?'
        this.allowButton = this.$t("message.allowButton")
        this.notAllowButton = this.$t("message.notAllowButton")
    },
    onAllow() {
        console.info('click allow');
        callNativeHandler("EVENT_ALLOW", "allow");
    },
    onNotAllow() {
        console.info('click not allow');
        callNativeHandler("EVENT_NOT_ALLOW", "not allow");
    }
}
