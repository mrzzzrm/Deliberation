#include "ProgramImpl.h"

#include <iostream>
#include <map>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/FileUtils.h>

namespace deliberation
{
ProgramImpl::ProgramImpl(const std::vector<std::string> & paths)
    : glProgramName(0u)
{
    glProgramName = gl::glCreateProgram();
    AssertM(glProgramName != 0, "Failed to create GL program");

    auto typeByExtension = std::map<std::string, gl::GLenum>();

    typeByExtension["frag"] = gl::GL_FRAGMENT_SHADER;
    typeByExtension["vert"] = gl::GL_VERTEX_SHADER;

    for (auto & pathOrName : paths)
    {
        auto path =
            pathOrName; // TheResourcePathResolve::instance().resolve(pathOrName);

        auto dotPos = path.rfind(".");
        AssertM(
            dotPos != std::string::npos,
            "File '" + path + "' has no extension");

        auto extension = path.substr(dotPos + 1, std::string::npos);

        auto      shaderSource = FileToString(path);
        auto *    shaderSourceCStr = shaderSource.c_str();
        gl::GLint length = shaderSource.size();

        auto iter = typeByExtension.find(extension);
        AssertM(
            iter != typeByExtension.end(),
            "Unknown shader extension '" + extension + "'");

        auto glShaderName = glCreateShader(iter->second);
        AssertM(glShaderName > 0, "Failed to create shader");

        gl::glShaderSource(glShaderName, 1, &shaderSourceCStr, &length);
        gl::glCompileShader(glShaderName);

        gl::GLint compileStatus;
        gl::glGetShaderiv(glShaderName, gl::GL_COMPILE_STATUS, &compileStatus);
        if ((gl::GLboolean)compileStatus != gl::GL_TRUE)
        {
            std::cout << iter->second << " '" << path
                      << "' compilation error:" << std::endl;

            gl::GLint infoLogLength;
            gl::glGetShaderiv(
                glShaderName, gl::GL_INFO_LOG_LENGTH, &infoLogLength);

            std::vector<char> infoLogBuf(infoLogLength + 1);
            gl::glGetShaderInfoLog(
                glShaderName, infoLogBuf.size(), NULL, infoLogBuf.data());

            std::string infoLog(infoLogBuf.data(), infoLogBuf.size() - 1);
            std::cout << infoLog << std::endl;

            Fail("");
        }

        gl::glAttachShader(glProgramName, glShaderName);
    }

    gl::glLinkProgram(glProgramName);

    gl::GLint linkStatus;
    gl::glGetProgramiv(glProgramName, gl::GL_LINK_STATUS, &linkStatus);
    if ((gl::GLboolean)linkStatus != gl::GL_TRUE)
    {
        std::cout << "Program (";
        for (auto & path : paths)
        {
            std::cout << path << ", ";
        }
        std::cout << ") link error: " << std::endl;

        gl::GLint infoLogLength;
        gl::glGetProgramiv(
            glProgramName, gl::GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<char> infoLogBuf(infoLogLength + 1);
        gl::glGetProgramInfoLog(
            glProgramName, infoLogBuf.size(), NULL, infoLogBuf.data());

        std::string infoLog(infoLogBuf.data(), infoLogBuf.size() - 1);
        std::cout << infoLog << std::endl;

        Fail("");
    }

    interface = ProgramInterface(glProgramName);
}
}
