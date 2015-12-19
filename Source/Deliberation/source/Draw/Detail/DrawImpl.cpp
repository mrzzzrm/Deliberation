#include "DrawImpl.h"

#include <algorithm>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include "ProgramImpl.h"

namespace deliberation
{

namespace detail
{

DrawImpl::DrawImpl(Context & context,
                   Program & program):
    context(context),
    program(program.m_impl),
    indexBuffer(nullptr),
    glVertexArray(0u)
{
    /*
        TODO
            This stuff could be cached per program
    */

    // Create UniformHandleImpls
    {
        uniforms.reserve(this->program->interface.uniforms().size());
        for (auto & uniform : this->program->interface.uniforms())
        {
            uniforms.emplace_back(uniform.type(), uniform.location());
        }
    }

    // Create Samplers
    {
        auto numSamplers = this->program->interface.samplers().size();

        std::vector<gl::GLuint> glNames;
        glNames.resize(numSamplers, 0u);
        gl::glGenSamplers(numSamplers, glNames.data());

        samplers.reserve(numSamplers);
        for (auto s = 0u; s < this->program->interface.samplers().size(); s++)
        {
            auto & sampler = this->program->interface.samplers()[s];
            Assert(glNames[s] != 0, "");
            samplers.emplace_back(glNames[s], sampler.type(), sampler.location());
        }
    }
}

}

}



//    for (auto & uniform : m_program.layout().uniforms())
//    {
//        d_unsetUniformLocations.insert(uniform.location());
//    }
