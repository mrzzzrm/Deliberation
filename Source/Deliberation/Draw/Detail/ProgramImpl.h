#pragma once

#include <string>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{
class DrawContext;

class ProgramImpl final
{
public:
    ProgramImpl(const std::vector<std::string> & paths);

    gl::GLuint       glProgramName;
    ProgramInterface interface;
};
}
