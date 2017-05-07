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
    ProgramImpl(
        DrawContext & drawContext, const std::vector<std::string> & paths);

    DrawContext &    drawContext;
    gl::GLuint       glProgramName;
    ProgramInterface interface;
};
}
