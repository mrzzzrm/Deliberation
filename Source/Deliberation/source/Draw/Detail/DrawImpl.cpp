#include "DrawImpl.h"

#include <algorithm>
#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include "ProgramImpl.h"

namespace deliberation
{

namespace detail
{

DrawImpl::DrawImpl(Context & context,
                   const Program & program):
    context(context),
    program(program.m_impl),
    glVertexArray(0u)
{
    /*
        TODO
            This stuff could be cached per program
    */

    // Create UniformImpls
    {
        uniforms.reserve(this->program->interface.uniforms().size());
        for (auto & uniform : this->program->interface.uniforms())
        {
            uniforms.emplace_back(uniform.type(), uniform.location(), uniform.arraySize());
        }
    }

    // Create Samplers
    {
        auto numSamplers = this->program->interface.samplers().size();

        samplers.reserve(numSamplers);
        for (auto s = 0u; s < this->program->interface.samplers().size(); s++)
        {
            auto & sampler = this->program->interface.samplers()[s];
            samplers.emplace_back(sampler.type(), sampler.valueType(), sampler.location());
        }
    }

    // Create framebuffer
    framebuffer = context.backbuffer();

    // Allocate Uniform Buffer Bindings
    uniformBuffers.resize(program.interface().uniformBlocks().size());
}

}

}

