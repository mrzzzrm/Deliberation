#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Detail/UniformImpl.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/DrawState.h>

#include "BufferBinding.h"
#include "SamplerImpl.h"
#include "UniformBufferBinding.h"

namespace deliberation
{

class Context;
class Program;

namespace detail
{

class ProgramImpl;

class DrawImpl final
{
public:
    DrawImpl(Context & context,
             const Program & program);

    Context &                                   context;
    std::string                                 name;
    std::shared_ptr<ProgramImpl>                program;
    std::shared_ptr<BufferImpl>                 indexBuffer;
    std::vector<BufferBinding>                  vertexBuffers;
    std::vector<BufferBinding>                  instanceBuffers;
    DrawState                                   state;
    Framebuffer                                 framebuffer;
    gl::GLuint                                  glVertexArray;
    std::vector<UniformImpl>                    uniforms;
    std::vector<SamplerImpl>                    samplers;
    std::vector<Optional<UniformBufferBinding>> uniformBuffers;
};

}

}

