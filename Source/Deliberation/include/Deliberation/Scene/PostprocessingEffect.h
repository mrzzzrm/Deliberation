#pragma once

#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{

class Context;

class DELIBERATION_API PostprocessingEffect final
{
public:
    PostprocessingEffect();
    PostprocessingEffect(Context & context,
                         const std::vector<std::string> & shaders,
                         const std::string & name = std::string());

    Draw & draw();
    const Draw & draw() const;

    void schedule();

private:
    bool        m_initialised;
    Draw        m_draw;
    Program     m_program;
    Buffer      m_vertexBuffer;
};

}

