#include <Deliberation/ECS/ActivityManager.h>

#include <algorithm>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

std::shared_ptr<AbstractActivity>
ActivityManager::createActivity(const std::string & name)
{
    const auto iter = m_activityFactoryByName.find(name);
    AssertM(
    iter != m_activityFactoryByName.end(),
    "No such Behaviour '" + name + "'");

    return iter->second();
}

void ActivityManager::addActivity(const std::shared_ptr<AbstractActivity> & activity)
{
    for (const auto & invoker : activity->m_phaseInvokers)
    {
        addInvoker(invoker);
    }
    m_activities.emplace_back(activity);
    activity->m_manager = shared_from_this();

    activity->onCreated();
}

void ActivityManager::removeActivity(const std::shared_ptr<AbstractActivity> & activity)
{
    activity->onRemoved();

    for (const auto & invoker : activity->m_phaseInvokers)
    {
        removeInvoker(invoker);
    }

    activity->m_manager.reset();

    auto iter = std::find(m_activities.begin(), m_activities.end(), activity);
    AssertF(iter != m_activities.end(), "Activity '{}' not registered", activity->name());

    m_activities.erase(iter);
}

void ActivityManager::addInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker)
{
    auto iter = m_containersByPhaseTypeId.find(invoker->phaseTypeId());
    if (iter == m_containersByPhaseTypeId.end())
    {
        iter = m_containersByPhaseTypeId.emplace(invoker->phaseTypeId(), invoker->createContainer()).first;
    }

    iter->second->addInvoker(invoker);
}

void ActivityManager::removeInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker)
{
    auto iter = m_containersByPhaseTypeId.find(invoker->phaseTypeId());
    Assert(iter != m_containersByPhaseTypeId.end());

    iter->second->removeInvoker(invoker);

}

}
