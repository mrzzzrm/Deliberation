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
    auto iter = m_containersByPhaseTypeId.find(TypeID::value<ActivityManager, T>());
    Assert(iter != m_containersByPhaseTypeId.end());

    const auto & abstractContainer = iter->second;
    const auto container = std::static_pointer_cast<typename T::Container>(abstractContainer);

    container->invokeAll(std::forward<Args>(args)...);
}

}