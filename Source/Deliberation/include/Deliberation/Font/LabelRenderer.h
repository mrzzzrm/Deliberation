#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/Sampler.h>



namespace deliberation
{

class DrawContext;
class Label;
class Viewport;

class LabelRenderer final
{
public:
    LabelRenderer();
    LabelRenderer(DrawContext & drawContext);

    /*
        TODO
            Draw/DrawContext should insert the Viewport
    */
    void render(const Label & label, const Viewport & viewport);

//private:
    DrawContext *       m_drawContext;
    Program         m_program;
    Buffer          m_vertexBuffer;
    Draw            m_draw;
    Uniform         m_colorUniform;
    Uniform         m_transformUniform;
    Sampler         m_sampler;
};

}

