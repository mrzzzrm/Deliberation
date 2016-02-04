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

class Context;

namespace detail
{

class ProgramImpl final
{
public:
    ProgramImpl(Context & context, const std::vector<std::string> & paths);

    Context &           context;
    gl::GLuint          glProgramName;
    ProgramInterface    interface;
};

}

}

