#pragma once

namespace deliberation
{
class RenderManager;

class RenderNode
{
  public:
    RenderNode(RenderManager & renderManager) : m_renderManager(renderManager)
    {
    }
    virtual ~RenderNode() = default;

    virtual void render() = 0;

  protected:
    RenderManager & m_renderManager;
};
}