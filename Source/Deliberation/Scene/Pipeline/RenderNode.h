#pragma once

#include <memory>

namespace deliberation
{
class Renderer;
class RenderManager;

class RenderNode
{
public:
    explicit RenderNode(const std::weak_ptr<Renderer> & renderer) :
        m_renderer(renderer)
    {
    }

    virtual ~RenderNode() = default;

    bool enabled() const { return m_enabled; }
    std::shared_ptr<Renderer> renderer() { return m_renderer.lock(); }

    void setEnabled(bool enabled) { m_enabled = enabled; }

    virtual void        render() = 0;
    virtual std::string name() const { return "Unnamed"; };

protected:
    std::weak_ptr<Renderer> m_renderer;
    bool m_enabled = true;
};
}