#pragma once

#include <functional>
#include <string>
#include <unordered_set>

#include <Deliberation/Draw/GL/GLSLValue.h>
#include <Deliberation/Draw/DrawState.h>

namespace deliberation
{

class Buffer;
class Context;
class Program;

namespace detail
{

class DrawImpl final
{
public:
    struct BufferBinding
    {
        std::reference_wrapper<const Buffer> buffer;
        bool ranged;
        unsigned int first;
        unsigned int count;
        unsigned int divisor;
    };

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
    std::vector<GLSLValue> m_uniforms;
    std::vector<BufferBinding> vertexBuffers;
    std::vector<BufferBinding> instanceBuffers;
    DrawState state;

public:
    std::unordered_set<unsigned int> d_unsetUniformLocations;


};

}

}
