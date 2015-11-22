#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/DrawState.h>

#include "Draw/GL/GLSLValue.h"

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
//    std::vector<GLSLValue> m_uniforms;
    std::vector<BufferBinding> vertexBuffers;
    std::vector<BufferBinding> instanceBuffers;
    DrawState state;
    gl::GLuint glVertexArray;


public:
    std::unordered_set<unsigned int> d_unsetUniformLocations;
};

}

}
