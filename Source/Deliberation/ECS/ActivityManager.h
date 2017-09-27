#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/AbstractActivity.h>

namespace deliberation
{

class ActivityManager : public std::enable_shared_from_this<ActivityManager>
{
public:
    // Internal - just use for debugging
    const std::vector<std::shared_ptr<AbstractActivity>> & activities() const { return m_activities; }

    template<typename T>
    void addActivityType(const std::string & name);

    std::shared_ptr<AbstractActivity>
    createActivity(const std::string & name);

    void addActivity(const std::shared_ptr<AbstractActivity> & activity);
    void removeActivity(const std::shared_ptr<AbstractActivity> & activity);

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
    std::vector<std::shared_ptr<AbstractActivity>> m_activities;

    std::unordered_map<
        std::string,
        std::function<std::shared_ptr<AbstractActivity>()>> m_activityFactoryByName;
};

}

#include <Deliberation/ECS/ActivityManager.inl>