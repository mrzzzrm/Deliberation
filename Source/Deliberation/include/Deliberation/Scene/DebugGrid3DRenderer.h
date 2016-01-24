#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{

class Context;
class Camera3D;

class DebugGrid3DRenderer final
{
public:
    DebugGrid3DRenderer(Context & context,
                        float scale,
                        const Camera3D & camera);

    void draw();

private:
    void init(float scale);

private:
    Context &           m_context;
    const Camera3D &    m_camera;
    Draw                m_normalLines;
    Draw                m_fatLines;
    Program             m_program;
    Buffer              m_vertexBuffer;
};

}

