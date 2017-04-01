#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/SkyboxRenderer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class SkyboxSystem:
    public System<SkyboxSystem>
{
public:
    SkyboxSystem(World & world, const Camera3D & camera, const Texture & cubeMap):
        Base(world),
        m_renderer(world.system<ApplicationSystem>().context(), camera, cubeMap)
    {}

protected:
    void onRender() override
    {
        m_renderer.render();
    }

private:
    SkyboxRenderer m_renderer;
};

}