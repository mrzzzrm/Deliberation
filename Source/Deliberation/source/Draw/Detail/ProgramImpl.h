#pragma once

#include <string>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/ProgramInterface.h>

namespace globjects
{
    class Program;
}

namespace deliberation
{

class DrawContext;

namespace detail
{

class ProgramImpl final
{
public:
    ProgramImpl(DrawContext & drawContext, const std::vector<std::string> & paths);

    DrawContext &           drawContext;
    gl::GLuint          glProgramName;
    ProgramInterface    interface;
};

}

}

