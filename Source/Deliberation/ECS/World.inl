#include <type_traits>

#include <Deliberation/ECS/ActivityManager.h>
#include <Deliberation/ECS/System.h>

namespace deliberation
{
template<typename T>
std::shared_ptr<T> World::system()
{
    AssertM((std::is_base_of<SystemBase, T>::value), "");

    const auto index = T::indexStatic();

    if (!m_systems.contains(index)) return {};

    auto systemBase = m_systems.at(index);
    return std::static_pointer_cast<T>(systemBase);
}

template<typename T>
T & World::systemRef()
{
    auto systemPtr = system<T>();
    AssertM((bool)systemPtr, "No such system");
    return *systemPtr;
}

template<typename T, typename... Args>
std::shared_ptr<T> World::addSystem(Args &&... args)
{
    AssertM((std::is_base_of<SystemBase, T>::value), "");
    AssertM(m_entities.empty(), "Can't add Systems once entities exist"); // We'd have to scan all entities again and add them to the World otherwise

    auto system = std::make_shared<T>(*this, std::forward<Args>(args)...);
    m_systems.emplace(system->index(), system);

    m_activityManager->addActivity(system);

    return system;
}
}
