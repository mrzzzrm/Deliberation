#pragma once

#include <memory>

namespace deliberation
{

class DrawContext;
class RenderManager;

class Renderer:
    public std::enable_shared_from_this<Renderer>
{
public:
    Renderer(RenderManager & renderManager): m_renderManager(renderManager) {}
    virtual ~Renderer() = default;

    RenderManager & renderManager() const { return m_renderManager; }
    DrawContext & drawContext() const;

    virtual void registerRenderNodes() = 0;

protected:
    RenderManager & m_renderManager;
};

}