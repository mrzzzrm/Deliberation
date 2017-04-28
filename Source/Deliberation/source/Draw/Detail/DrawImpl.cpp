#include "DrawImpl.h"

#include <algorithm>
#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include "ProgramImpl.h"

namespace deliberation
{

namespace detail
{

DrawImpl::DrawImpl(DrawContext & drawContext,
                   const Program & program):
    drawContext(drawContext),
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
    framebuffer = drawContext.backbuffer();

    // Allocate Uniform Buffer Bindings
    uniformBuffers.resize(this->program->interface.uniformBlocks().size());
    
    // Allocate vertex attribute bindings
    attributeBindings.resize(this->program->interface.attributes().size());
    valueAttributes = Blob(0);
}

}

}

