#pragma once

#include <string>

#include <glbinding/gl/types.h>

namespace deliberation
{
gl::GLint GLGetProgram(gl::GLuint glProgramName, gl::GLenum pname);
std::string
GLGetActiveUniformName(gl::GLuint glProgramName, gl::GLuint uniformIndex);
std::string
          GLGetActiveUniformBlockName(gl::GLuint glProgramName, gl::GLuint blockIndex);
gl::GLint GLGetActiveUniformBlockInt(
    gl::GLuint glProgramName, gl::GLuint blockIndex, gl::GLenum pname);
}
