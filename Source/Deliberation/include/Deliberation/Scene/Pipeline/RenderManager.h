#pragma once

#include <memory>
#include <vector>

#include <Deliberation/ECS/System.h>

namespace deliberation
{

class DrawContext;
class Renderer;

class RenderManager:
    public System<RenderManager>
{
public:
    RenderManager(World & world, DrawContext & context);
    virtual ~RenderManager() = default;

    // Callbacks for Renderers
    void registerRenderer(const std::shared_ptr<Renderer> & renderer);
    void registerRenderNode(const std::shared_ptr<RenderNode> & node);

protected:
    virtual void onUpdate(float seconds) override;

private:
    std::vector<std::vector<
};

}