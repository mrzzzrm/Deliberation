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
    virtual std::string name() const { return "Unnamed"; };

  protected:
    RenderManager & m_renderManager;
};
}