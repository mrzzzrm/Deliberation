#pragma once

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class Camera3D;
class DrawContext;
class RenderManager;

class SkyboxRenderer : public SingleNodeRenderer
{
public:
    SkyboxRenderer(const Texture & cubemap);

    void render() override;

private:
    Draw    m_draw;
    bool    m_drawDirty = true;
    Program m_program;
    Texture m_cubemap;
};
}