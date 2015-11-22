#include "ProgramImpl.h"

#include <map>

#include <glbinding/gl/enum.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

ProgramImpl::ProgramImpl(const std::vector<std::string> & paths)
{
    globjectsProgram = globjects::make_ref<globjects::Program>();

    auto typeByExtension = std::map<std::string, gl::GLenum>();

    typeByExtension["frag"] = gl::GL_FRAGMENT_SHADER;
    typeByExtension["vert"] = gl::GL_VERTEX_SHADER;

    for (auto & pathOrName : paths)
    {
        auto path = pathOrName; //TheResourcePathResolve::instance().resolve(pathOrName);

        auto dotPos = path.rfind(".");
        Assert(dotPos != std::string::npos, "File '" + path + "' has no extension");

        auto extension = path.substr(dotPos + 1, std::string::npos);

        auto iter = typeByExtension.find(extension);
        Assert(iter != typeByExtension.end(), "Unknown shader extension '" + extension + "'");

        globjectsProgram->attach(globjects::Shader::fromFile(iter->second, path));
    }

    globjectsProgram->link();

    interface = ProgramInterface(*globjectsProgram);
}

}

}

