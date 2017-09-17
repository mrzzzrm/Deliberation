#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{
class PointLightRenderer;
class DebugGeometryNode;

class DebugPointLightSystem : public System<DebugPointLightSystem>
{
public:
    DebugPointLightSystem(
        World &                                     world,
        const std::shared_ptr<PointLightRenderer> & pointLightRenderer);

    void onGameUpdate(const UpdateFrame & updateFrame);

private:
    std::shared_ptr<DebugGeometryNode>  m_node;
    std::shared_ptr<PointLightRenderer> m_pointLightRenderer;
    size_t                              m_numPointLights = 0;
};
}