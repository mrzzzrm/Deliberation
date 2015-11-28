#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Detail/UniformImpl.h>
#include <Deliberation/Draw/DrawState.h>

#include "BufferBinding.h"

namespace deliberation
{

class Context;
class Program;

namespace detail
{

class DrawImpl final
{
public:
//    struct TextureUnitBinding
//    {
//        const globjects::Texture * texture;
//        const globjects::Sampler * sampler;
//    };

public:
    DrawImpl(Context & context,
             Program & program);

    Context & context;
    Program & program;
    std::string name;
    const Buffer * indexBuffer;
    std::vector<BufferBinding> vertexBuffers;
    std::vector<BufferBinding> instanceBuffers;
    DrawState state;
    gl::GLuint glVertexArray;

    std::vector<UniformImpl> uniforms;
};

}

}

