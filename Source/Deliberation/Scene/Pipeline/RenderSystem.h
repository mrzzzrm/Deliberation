#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{
class RenderSystem : public System<RenderSystem>
{
  public:
    RenderSystem(World & world);

    RenderManager & renderManager() { return m_renderManager; }

  protected:
    void onUpdate(float seconds) override;

  private:
    RenderManager m_renderManager;
};
}