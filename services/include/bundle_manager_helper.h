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

#ifndef FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_BUNDLE_MANAGER_HELPER_H
#define FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_BUNDLE_MANAGER_HELPER_H

#include <string>
#include <vector>

#include "bms_death_recipient.h"
#include "bundle_mgr_interface.h"
#include "extension_ability_info.h"
#include "iremote_object.h"
#include "refbase.h"
#include "singleton.h"
#include "want.h"

namespace OHOS {
namespace EventFwk {
class BundleManagerHelper : public DelayedSingleton<BundleManagerHelper> {
public:
    using IBundleMgr = OHOS::AppExecFwk::IBundleMgr;

    BundleManagerHelper();

    virtual ~BundleManagerHelper();

    bool CheckIsSystemAppByUid(uid_t uid);

    std::string GetBundleName(int uid);

    bool QueryExtensionInfos(std::vector<AppExecFwk::ExtensionAbilityInfo> &extensionInfo);

    bool QueryExtensionInfos(std::vector<AppExecFwk::ExtensionAbilityInfo> &extensionInfos, const int32_t &userId);

    bool GetResConfigFile(const AppExecFwk::ExtensionAbilityInfo &extension, std::vector<std::string> &profileInfos);

    bool CheckPermission(const std::string &bundleName, const std::string &permission);

    void ClearBundleManagerHelper();

private:
    bool GetBundleMgrProxy();

private:
    sptr<IBundleMgr> sptrBundleMgr_;
    std::mutex mutex_;
    sptr<BMSDeathRecipient> bmsDeath_;
};
}  // namespace EventFwk
}  // namespace OHOS

#endif  // FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_BUNDLE_MANAGER_HELPER_H