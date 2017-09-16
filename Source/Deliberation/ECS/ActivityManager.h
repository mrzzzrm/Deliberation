#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/UntypedActivity.h>

namespace deliberation
{

class ActivityManager : public std::enable_shared_from_this<ActivityManager>
{
public:
    void addActivity(const std::shared_ptr<UntypedActivity> & activity);
    void removeActivity(const std::shared_ptr<UntypedActivity> & activity);

    template<typename T, typename ... Args>
    void invokePhase(Args &&... args);

private:
    template<typename T> friend class Activity;

private:
    void addInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker);
    void removeInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker);

private:
    std::unordered_map<TypeID::value_t,
     std::shared_ptr<AbstractPhaseContainer>> m_containersByPhaseTypeId;
    std::vector<std::shared_ptr<UntypedActivity>> m_activities;

};

}

#include <Deliberation/ECS/ActivityManager.inl>