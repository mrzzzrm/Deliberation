#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{

class Camera3D;
class Context;
class Framebuffer;

class DELIBERATION_API DebugGroundPlaneRenderer final
{
public:
    DebugGroundPlaneRenderer(Context & context, const Camera3D & camera);

    void setFramebuffer(Framebuffer & framebuffer);
    void setSize(float size);
    void setQuadSize(float quadSize);

    void schedule();

private:
    Context &           m_context;
    const Camera3D &    m_camera;

    Program             m_program;
    Draw                m_draw;
    Uniform             m_view;
    Uniform             m_projection;
    Uniform             m_farPlaneZ;
    Uniform             m_size;
    Uniform             m_quadSize;
};

}

