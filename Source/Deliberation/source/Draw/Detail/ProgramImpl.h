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

namespace detail
{

class ProgramImpl final
{
public:
    ProgramImpl(const std::vector<std::string> & paths);

    gl::GLuint          glProgramName;
    ProgramInterface    interface;
};

}

}

