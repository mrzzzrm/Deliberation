#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{
class DrawContext;
class Camera3D;

class DebugGrid3DRenderer final
{
public:
    DebugGrid3DRenderer(float scale, const Camera3D & camera);

    void setFramebuffer(deliberation::Framebuffer & fb);

    void draw();

private:
    void init(float scale);

private:
    const Camera3D & m_camera;
    Draw             m_normalLines;
    Draw             m_fatLines;
    Program          m_program;
    Buffer           m_vertexBuffer;
};
}
