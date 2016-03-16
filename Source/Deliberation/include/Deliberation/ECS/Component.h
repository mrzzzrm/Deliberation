#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/ComponentBase.h>

namespace deliberation
{

class World;

template<typename T>
class Component:
    public ComponentBase
{
public:
    static std::size_t indexStatic();

public:
    virtual std::size_t index() const override;
    virtual std::string name() const override;

private:
    friend class World;
};

}

#include <Deliberation/ECS/Component.inl>

