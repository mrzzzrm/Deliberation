#pragma once

#include <algorithm>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Swallow.h>

#include <Deliberation/ECS/Activity.h>
#include <Deliberation/ECS/ActivityManager.h>

namespace deliberation
{

template<typename T>
Activity<T>::Activity(const std::string & name):
UntypedActivity(name)
{
}

template<typename T>
template<typename ... Args>
void Activity<T>::activatePhases()
{
    Swallow((activatePhase<Args>(), 0)...);
}

template<typename T>
template<typename ... Args>
void Activity<T>::deactivatePhases()
{
    Swallow((deactivatePhase<Args>(), 0)...);
}

template<typename T>
template<typename P>
void Activity<T>::activatePhase()
{
    const auto invoker = P::createInvoker(*static_cast<T*>(this));
    m_phaseInvokers.emplace_back(invoker);

    auto manager = m_manager.lock();
    if (manager)
    {
        manager->addInvoker(invoker);
    }
}

template<typename T>
template<typename P>
void Activity<T>::deactivatePhase()
{
    const auto phaseTypeId = TypeID::value<ActivityManager, P>();

    auto iter = std::find_if(m_phaseInvokers.begin(), m_phaseInvokers.end(), [&](const auto & rhs) {
        return rhs->phaseTypeId() == phaseTypeId;
    });
    Assert(iter != m_phaseInvokers.end());

    auto manager = m_manager.lock();
    if (manager)
    {
        manager->removeInvoker(*iter);
    }

    m_phaseInvokers.erase(iter);
}

}