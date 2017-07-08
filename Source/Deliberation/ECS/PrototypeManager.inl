#include <utility>

namespace deliberation
{

template<typename T, typename ... Args>
void PrototypeManager::registerComponentPrototype(const std::string & name, Args &&... args)
{
    m_componentPrototypeFactoryByName.emplace(name, [name, args...]() {
        auto prototype = std::make_shared<T>(args...);
        prototype->setName(name);
        return prototype;
    });
}

}