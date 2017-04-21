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
    ProgramImpl(DrawContext & context, const std::vector<std::string> & paths);

    DrawContext &           context;
    gl::GLuint          glProgramName;
    ProgramInterface    interface;
};

}

}

