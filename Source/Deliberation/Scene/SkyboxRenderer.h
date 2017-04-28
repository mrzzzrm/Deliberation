#pragma once

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Camera3D;
class DrawContext;

class SkyboxRenderer final
{
public:
    SkyboxRenderer(DrawContext & drawContext, const Camera3D & camera, const Texture & cubemap);

    void render();

private:
    const Camera3D &    m_camera;
    Draw                m_draw;
};

}