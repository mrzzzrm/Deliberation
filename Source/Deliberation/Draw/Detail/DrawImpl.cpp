#include "DrawImpl.h"

#include <algorithm>
#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/GL/GLType.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include "ProgramImpl.h"

namespace deliberation
{
DrawImpl::DrawImpl(DrawContext & drawContext, const Program & program)
    : drawContext(drawContext), program(program.m_impl), glVertexArray(0u)
{
    /*
        TODO
            This stuff could be cached per program
    */

    // Create UniformImpls
    uniforms.resize(this->program->interface.uniforms().size());

    std::vector<DataLayoutField::Desc> uniformFieldsDescs;
    uniformFieldsDescs.reserve(this->program->interface.uniforms().size());
    for (const auto & uniform : this->program->interface.uniforms())
    {
        uniformFieldsDescs.emplace_back(
            uniform.name(), uniform.type(), uniform.arraySize());
    }

    uniformLayout = DataLayout(uniformFieldsDescs);
    uniformData = LayoutedBlob(uniformLayout, 1);

    // Create Samplers
    {
        auto numSamplers = this->program->interface.samplers().size();
        samplers.reserve(numSamplers);

        for (auto s = 0u; s < numSamplers; s++)
        {
            auto & sampler = this->program->interface.samplers()[s];
            samplers.emplace_back(std::make_shared<SamplerImpl>(
                (gl::GLenum)sampler.type(),
                TypeToGLType(sampler.valueType()),
                sampler.location()));
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

DrawImpl::~DrawImpl()
{
    if (glVertexArray != 0)
    {
        drawContext.m_glStateManager.deleteVertexArray(glVertexArray);
    }
}

void DrawImpl::setAttribute(
    const ProgramInterfaceVertexAttribute & attribute, const void * data)
{
    auto & binding = attributeBindings[attribute.index()];

    if (binding.which() ==
        0) // Attribute not yet assigned, allocate new space in value blob
    {
        const auto offset = valueAttributes.size();
        valueAttributes.resize(offset + attribute.type().size());

        auto valueBinding = VertexAttributeValueBinding();
        valueBinding.offset = offset;
        valueBinding.attributeIndex = attribute.index();

        binding = valueBinding;
    }

    Assert(
        binding.which() != 2,
        "Vertex attribute '" + attribute.name() +
            "' is already bound to buffer");

    auto & valueBinding = boost::get<VertexAttributeValueBinding>(binding);

    valueAttributes.write(valueBinding.offset, data, attribute.type().size());
    dirtyValueAttributes.emplace_back(attribute.index());
}
}
