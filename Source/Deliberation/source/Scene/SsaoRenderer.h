#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{

class SsaoRenderer:
    public Renderer
{
public:
    SsaoRenderer(RenderManager & renderManager);

    void registerRenderNodes() override;
};

}