#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Detail/UniformImpl.h>
#include <Deliberation/Draw/Detail/VertexAttributeBinding.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/DrawState.h>
#include <Deliberation/Draw/VertexAttribute.h>

#include "AttributeBinding.h"
#include "BufferBinding.h"
#include "SamplerImpl.h"
#include "UniformBufferBinding.h"

namespace deliberation
{

class DrawContext;
class Program;

namespace detail
{

class ProgramImpl;

class DrawImpl final
{
public:
    DrawImpl(DrawContext & drawContext,
             const Program & program);

    DrawContext &                                   drawContext;
    std::string                                 name;
    std::shared_ptr<ProgramImpl>                program;
    BufferBinding                               indexBufferBinding;
    bool                                        indexBufferBindingDirty = true;

    /**
     * Vertex attributes
     */
    std::vector<VertexAttributeBinding>         attributeBindings;
    std::vector<u32>                            dirtyValueAttributes;
    Blob                                        valueAttributes;

    DrawState                                   state;
    Framebuffer                                 framebuffer;
    gl::GLuint                                  glVertexArray;
    std::vector<UniformImpl>                    uniforms;
    std::vector<SamplerImpl>                    samplers;
    std::vector<Optional<UniformBufferBinding>> uniformBuffers;
};

}

}

