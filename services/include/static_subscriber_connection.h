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

#ifndef FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_STATIC_SUBSCRIBER_CONNECTION_H
#define FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_STATIC_SUBSCRIBER_CONNECTION_H

#include "ability_connect_callback_stub.h"
#include "common_event_data.h"
#include "static_subscriber_proxy.h"

namespace OHOS {
namespace EventFwk {
class StaticSubscriberConnection : public AAFwk::AbilityConnectionStub {
public:
    explicit StaticSubscriberConnection(const CommonEventData& event) : event_(event) {}
    void OnAbilityConnectDone(
        const AppExecFwk::ElementName &element, const sptr<IRemoteObject> &remoteObject, int resultCode) override;

    void OnAbilityDisconnectDone(const AppExecFwk::ElementName &element, int resultCode) override;
private:
    sptr<AppExecFwk::StaticSubscriberProxy> proxy_ = nullptr;
    CommonEventData event_;
};
}  // namespace EventFwk
}  // namespace OHOS
#endif  // FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_STATIC_SUBSCRIBER_CONNECTION_H