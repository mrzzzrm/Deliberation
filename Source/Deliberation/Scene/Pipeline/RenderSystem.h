#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{
class RenderSystem : public System<RenderSystem>
{
public:
    RenderSystem(World & world);

    RenderManager & renderManager() { return m_renderManager; }
    const std::shared_ptr<DebugGeometryRenderer> & debugGeometryRenderer() const
    {
        return m_debugGeometryRenderer;
    }

protected:
    void onGameUpdate(float seconds) override;

private:
    RenderManager                          m_renderManager;
    std::string                            m_selectedSurfaceKey;
    std::shared_ptr<DebugGeometryRenderer> m_debugGeometryRenderer;
    std::string                            m_imguiViewTag = "Rendering";
};
}