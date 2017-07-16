#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

namespace deliberation
{
class ImGuiRenderer : public SingleNodeRenderer
{
public:
    ImGuiRenderer(RenderManager & renderManager);

    void render() override;

protected:
    void onSetupRender() override;

private:
    Draw    m_draw;
    Buffer  m_vertexBuffer;
    Buffer  m_indexBuffer;
    Uniform m_projectionUniform;

    unsigned char * m_fontAtlasPixels;
    int             m_fontAtlasWidth;
    int             m_fontAtlasHeight;
};
}