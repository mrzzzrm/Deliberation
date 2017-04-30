#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class SkyboxSystem:
    public System<SkyboxSystem>
{
public:
    SkyboxSystem(World & world, const Texture & cubeMap):
        Base(world)
    {
        world.systemRef<RenderSystem>().renderManager().addRenderer<SkyboxRenderer>(cubeMap);
    }
};

}