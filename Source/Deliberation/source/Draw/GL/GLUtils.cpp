#include "GLUtils.h"

#include <vector>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

gl::GLint GLGetProgram(gl::GLuint glProgramName, gl::GLenum pname)
{
    gl::GLint value = 0;
    gl::glGetProgramiv(glProgramName, pname, &value);
	return value;
}

std::string GLGetActiveUniformName(gl::GLuint glProgramName, gl::GLuint uniformIndex)
{
    gl::GLint length;
    glGetActiveUniformsiv(glProgramName, 1, &uniformIndex, gl::GL_UNIFORM_NAME_LENGTH, &length);
    Assert(length > 1, ""); // Has to include at least 1 char and '\0'

    std::vector<char> name(length);
    gl::glGetActiveUniformName(glProgramName, uniformIndex, length, nullptr, name.data());

	// glGetActiveUniformName() insists we query '\0' as well, but it
	// shouldn't be passed to std::string(), otherwise std::string::size()
	// returns <actual size> + 1 (on clang)
    auto numChars = length - 1;
    return std::string(name.data(), numChars);
}

}

