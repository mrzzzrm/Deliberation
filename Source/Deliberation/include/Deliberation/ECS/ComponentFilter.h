#pragma once

#include <bitset>
#include <vector>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>
#include <Deliberation/ECS/Entity.h>

namespace deliberation
{

class ComponentFilter final
{
public:
    static ComponentFilter all();
    static ComponentFilter none();

public:
    template<typename ... Args>
    ComponentFilter();

    bool accepts(const ComponentBitset & componentBits) const;

private:
    ComponentBitset m_components;
};

}

#include <Deliberation/ECS/ComponentFilter.inl>
