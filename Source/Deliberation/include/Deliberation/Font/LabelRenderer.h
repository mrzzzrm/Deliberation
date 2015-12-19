#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/Sampler.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Context;
class Label;
class Viewport;

class DELIBERATION_API LabelRenderer final
{
public:
    LabelRenderer();
    LabelRenderer(Context & context);

    /*
        TODO
            Draw/Context should insert the Viewport
    */
    void render(const Label & label, const Viewport & viewport);

//private:
    Context *       m_context;
    Program         m_program;
    Buffer          m_vertexBuffer;
    Draw            m_draw;
    Uniform         m_colorUniform;
    Uniform         m_transformUniform;
    Sampler         m_sampler;
};

}

