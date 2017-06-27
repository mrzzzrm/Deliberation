namespace deliberation
{

template<typename T, typename ... Args>
void PrototypeManager::registerComponentPrototype(const std::string & name, Args ... args)
{
    m_componentPrototypeFactoryByName.emplace(name, [=]() {
        auto prototype = std::make_shared<T>(args...);
        prototype->setName(name);
        return prototype;
    });
}

}