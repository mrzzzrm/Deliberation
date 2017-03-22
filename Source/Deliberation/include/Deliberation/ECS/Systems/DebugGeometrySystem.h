#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Debug/DebugGeometryManager.h>

#include <Deliberation/Deliberation.h>

class DebugGeometrySystem:
    public System<DebugGeometrySystem>
{
public:
    DebugGeometrySystem(World & world):
        Base(world),
        m_manager(world.system<ApplicationSystem>().context())
    {}

    DebugGeometryManager & manager() { return m_manager; }
    const DebugGeometryManager & manager() const { return m_manager; }

private:
    DebugGeometryManager m_manager;
};