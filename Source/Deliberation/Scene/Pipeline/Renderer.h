#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{
class DrawContext;
class RenderManager;

class Renderer : public std::enable_shared_from_this<Renderer>
{
  public:
    Renderer(
        RenderManager &     renderManager,
        const std::string & name = {"Unnamed Renderer"})
        : m_renderManager(renderManager), m_name(name)
    {
    }
    virtual ~Renderer() = default;

    RenderManager &     renderManager() const { return m_renderManager; }
    DrawContext &       drawContext() const;
    std::string name() const { return m_name; }

    virtual void registerRenderNodes() = 0;

    // Debug
    virtual void renderDebugGui() {}
    virtual std::vector<Framebuffer> framebuffers() { return {}; }

  protected:
    RenderManager & m_renderManager;

  private:
    std::string m_name;
};
}