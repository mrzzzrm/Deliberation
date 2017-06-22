namespace deliberation
{

template<typename T>
void PrototypeManager::registerComponentPrototype(const std::string & name)
{
    m_componentPrototypeFactoryByName.emplace(name, [=]() {
        auto prototype = std::make_shared<T>();
        prototype->setName(name);
        return prototype;
    });
}

}