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

#ifndef FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_COMMON_EVENT_SUBSCRIBER_MANAGER_H
#define FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_COMMON_EVENT_SUBSCRIBER_MANAGER_H

#include "common_event_constant.h"
#include "common_event_record.h"
#include "common_event_subscribe_info.h"
#include "iremote_object.h"
#include "singleton.h"

namespace OHOS {
namespace EventFwk {
struct EventRecordInfo {
    pid_t pid;
    uid_t uid;
    std::string bundleName;
    bool isSubsystem;
    bool isSystemApp;
    bool isProxy;

    EventRecordInfo() : pid(0), uid(0), isSubsystem(false), isSystemApp(false), isProxy(false) {}
};

struct EventSubscriberRecord {
    std::shared_ptr<CommonEventSubscribeInfo> eventSubscribeInfo;
    sptr<IRemoteObject> commonEventListener;
    EventRecordInfo eventRecordInfo;
    struct tm recordTime;
    bool isFreeze;
    int64_t freezeTime;

    EventSubscriberRecord()
        : eventSubscribeInfo(nullptr),
          commonEventListener(nullptr),
          isFreeze(false),
          freezeTime(0)
    {}
};

struct FrozenEventRecord {
    std::shared_ptr<EventSubscriberRecord> subscriberRecordPtr;
    std::vector<std::shared_ptr<CommonEventRecord>> eventRecordPtrs;

    FrozenEventRecord() : subscriberRecordPtr(nullptr)
    {}
};

inline bool operator<(const std::shared_ptr<EventSubscriberRecord> &a, const std::shared_ptr<EventSubscriberRecord> &b)
{
    return a->eventSubscribeInfo->GetPriority() > b->eventSubscribeInfo->GetPriority();
}

using SubscriberRecordPtr = std::shared_ptr<EventSubscriberRecord>;
using SubscribeInfoPtr = std::shared_ptr<CommonEventSubscribeInfo>;
using EventRecordPtr = std::shared_ptr<CommonEventRecord>;

class CommonEventSubscriberManager : public DelayedSingleton<CommonEventSubscriberManager> {
public:
    CommonEventSubscriberManager();

    virtual ~CommonEventSubscriberManager() override;

    bool InsertSubscriber(const SubscribeInfoPtr &eventSubscribeInfo, const sptr<IRemoteObject> &commonEventListener,
        const struct tm &recordTime, const EventRecordInfo &eventRecordInfo);

    int RemoveSubscriber(const sptr<IRemoteObject> &commonEventListener);

    std::vector<SubscriberRecordPtr> GetSubscriberRecords(const Want &want, const bool &isSystemApp,
        const int32_t &userId);

    void UpdateFreezeInfo(const uid_t &uid, const bool &freezeState, const int64_t &freezeTime = 0);

    void InsertFrozenEvents(const SubscriberRecordPtr &eventListener, const CommonEventRecord &eventRecord);

    std::map<SubscriberRecordPtr, std::vector<EventRecordPtr>> GetFrozenEvents(const uid_t &uid);

    void DumpDetailed(
        const std::string &title, const SubscriberRecordPtr &record, const std::string format, std::string &dumpInfo);

    void DumpState(const std::string &event, const int32_t &userId, std::vector<std::string> &state);

private:
    bool InsertSubscriberRecordLocked(const std::vector<std::string> &events, const SubscriberRecordPtr &record);

    int RemoveSubscriberRecordLocked(const sptr<IRemoteObject> &commonEventListener);

    bool CheckSubscriberByUserId(const int32_t &subscriberUserId, const bool &isSystemApp, const int32_t &userId);

    void GetSubscriberRecordsByWantLocked(const Want &want, const bool &isSystemApp, const int32_t &userId,
        std::vector<SubscriberRecordPtr> &records);

    void GetSubscriberRecordsByEvent(
        const std::string &event, const int32_t &userId, std::vector<SubscriberRecordPtr> &records);

    void RemoveFrozenEventsBySubscriber(const SubscriberRecordPtr &subscriberRecord);

    void RemoveFrozenEvents(const uid_t &uid);

private:
    std::mutex mutex_;
    sptr<IRemoteObject::DeathRecipient> death_;
    std::map<std::string, std::multiset<SubscriberRecordPtr>> eventSubscribers_;
    std::vector<SubscriberRecordPtr> subscribers_;
    std::map<uid_t, std::map<SubscriberRecordPtr, std::vector<EventRecordPtr>>> frozenEvents_;
    const time_t FREEZE_EVENT_TIMEOUT = 120; // How long we keep records. Unit: second
};
}  // namespace EventFwk
}  // namespace OHOS
#endif  // FOUNDATION_EVENT_CESFWK_SERVICES_INCLUDE_COMMON_EVENT_SUBSCRIBER_MANAGER_H