#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class AbstractPrototype;
class World;

class PrototypeManager:
    public std::enable_shared_from_this<PrototypeManager>
{
public:
    template<typename PrototypeType>
    void addPath(const std::string & path);

    template<typename PrototypeType>
    std::shared_ptr<const PrototypeType> prototype(const std::string & name) const;

    /**
     * Don't bail if the prototype with @name doesn't exist, instead create it
     * @tparam PrototypeType
     * @param name
     * @return
     */
    template<typename PrototypeType>
    std::shared_ptr<const PrototypeType> getOrCreatePrototype(const std::string & name);

    void reload();

private:
    struct PrototypeTypeContainer
    {
        PrototypeTypeContainer(const std::function<std::shared_ptr<AbstractPrototype>()> & prototypeFactory):
            prototypeFactory(prototypeFactory)
        {}

        std::vector<std::string> paths;
        std::unordered_map<std::string, std::shared_ptr<AbstractPrototype>>
            prototypeByName;
        std::function<std::shared_ptr<AbstractPrototype>()> prototypeFactory;
    };

private:
    std::unordered_map<TypeID::value_t, PrototypeTypeContainer>
        m_prototypeTypeContainersByTypeID;
};

}

#include "PrototypeManager.inl"