#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Sampler.h>
#include <Deliberation/Draw/Uniform.h>

namespace deliberation
{
class DrawContext;
class Label;
class Viewport;

class LabelRenderer final
{
public:
    LabelRenderer();

    /*
        TODO
            Draw/DrawContext should insert the Viewport
    */
    void render(const Label & label, const Viewport & viewport);

    // private:
    Program       m_program;
    Buffer        m_vertexBuffer;
    Draw          m_draw;
    Uniform       m_colorUniform;
    Uniform       m_transformUniform;
    Sampler       m_sampler;
};
}
