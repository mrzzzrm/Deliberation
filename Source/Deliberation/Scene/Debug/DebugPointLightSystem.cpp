#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/ECS/World.h>
#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

namespace deliberation
{
DebugPointLightSystem::DebugPointLightSystem(
    World &                                     world,
    const std::shared_ptr<PointLightRenderer> & pointLightRenderer)
    : Base(world), m_pointLightRenderer(pointLightRenderer)
{
    m_node = world.system<RenderSystem>()->debugGeometryRenderer()->addNode();
}

void DebugPointLightSystem::onGameUpdate(float seconds)
{
    for (size_t l = 0; l < m_pointLightRenderer->pointLights().size(); l++)
    {
        const auto & pointLight = m_pointLightRenderer->pointLight(l);

        m_node->sphere(l).setVisible((bool)pointLight.active);
        m_node->sphere(l).setTransform(
            Transform3D::atPosition(pointLight.position));
        m_node->sphere(l).setRadius(0.3f);
        m_node->sphere(l).setColor(
            !EpsilonEq(pointLight.intensity, glm::vec3(0))
                ? glm::normalize(pointLight.intensity)
                : glm::vec3(1.0f, 0.0f, 0.5f));
    }

    for (size_t l = m_pointLightRenderer->pointLights().size();
         l < m_numPointLights;
         l++)
    {
        m_node->removeSphere(l);
    }
    m_numPointLights = m_pointLightRenderer->pointLights().size();
}
}