#include <Deliberation/ECS/ActivityManager.h>

#include <memory>
#include <utility>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

template<typename T, typename ... Args>
void ActivityManager::invokePhase(Args &&... args)
{
    DELIBERATION_PROFILE_SCOPE("FrameBegin");

    auto iter = m_containersByPhaseTypeId.find(T::phaseTypeId());

    // No registered invokers in this phase - can happen, not an error
    if (iter == m_containersByPhaseTypeId.end()) return;

    const auto & abstractContainer = iter->second;
    const auto container = std::static_pointer_cast<typename T::Container>(abstractContainer);

    container->invokeAll(std::forward<Args>(args)...);
}

template<typename T>
void ActivityManager::addActivityType(const std::string &name)
{
    m_activityFactoryByName.emplace(name, [=]() {
        auto activity = std::make_shared<T>();
        activity->setName(name);
        return activity;
    });
}

}