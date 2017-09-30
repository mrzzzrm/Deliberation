#include <utility>

namespace deliberation
{
template<typename T, typename... Args>
void EntityPrototypeManager::registerComponentPrototype(
    const std::string & name, Args &&... args)
{
    m_componentPrototypeFactoryByName.emplace(name, [this, name, args...]() {
        auto prototype = std::make_shared<T>(args...);
        prototype->setName(name);
        prototype->setWorld(GetGlobal<World>());
        return prototype;
    });
}
}