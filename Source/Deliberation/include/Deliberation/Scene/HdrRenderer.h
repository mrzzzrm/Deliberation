#pragma once

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{

class HdrRenderer:
    public Renderer
{
public:
    HdrRenderer(RenderManager &renderManager);

    void registerRenderNodes() override;

private:
    friend class HdrNode;
};

}