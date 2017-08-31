#include <Deliberation/Draw/BufferTextureBinding.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Detail/BufferImpl.h>
#include <Deliberation/Draw/Detail/DrawImpl.h>
#include <Deliberation/Draw/Detail/ProgramImpl.h>
#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{
void BufferTextureBinding::setBuffer(const Buffer & buffer)
{
    auto drawImpl = m_drawImpl.lock();

    AssertM((bool)drawImpl, "Draw already expired or was never set");
    AssertM(
        buffer.layout().fields().size() == 1,
        "Buffer Textures are only allowed to have one field");

    auto & binding = drawImpl->bufferTextureBindings[m_index];

    binding.dirty = true;
    binding.ranged = false;
    binding.buffer = buffer;
    binding.internalFormat = internalFormatFromBuffer(buffer);
}

void BufferTextureBinding::setBufferRange(
    const Buffer & buffer, u32 begin, u32 count)
{
    auto drawImpl = m_drawImpl.lock();

    AssertM((bool)drawImpl, "Draw already expired or was never set");
    AssertM(
        buffer.layout().fields().size() == 1,
        "Buffer Textures are only allowed to have one field");
    AssertM(begin + count <= buffer.count(), "Setting buffer range out of range")
        AssertM(count > 0, "Can't set zero count");

    auto & binding = drawImpl->bufferTextureBindings[m_index];

    binding.dirty = true;
    binding.ranged = false;
    binding.begin = begin;
    binding.count = count;
    binding.buffer = buffer;
    binding.internalFormat = internalFormatFromBuffer(buffer);
}

gl::GLenum
BufferTextureBinding::internalFormatFromBuffer(const Buffer & buffer) const
{
    auto drawImpl = m_drawImpl.lock();

    const auto & interface =
        drawImpl->program->interface.bufferTextures()[m_index];

    const auto bufferType = buffer.layout().field(0).type();

    if (interface.type() == BufferTextureType::Float)
    {
        switch (bufferType.id())
        {
        case TYPE_U8: return gl::GL_R8;
        case TYPE_U16: return gl::GL_R16;
        case TYPE_U8VEC2: return gl::GL_RG8;
        case TYPE_U16VEC2: return gl::GL_RG16;
        case TYPE_U8VEC4: return gl::GL_RGBA8;
        case TYPE_U16VEC4: return gl::GL_RGBA16;
        case TYPE_FLOAT: return gl::GL_R32F;
        case TYPE_VEC2: return gl::GL_RG32F;
        case TYPE_VEC3: return gl::GL_RGB32F;
        case TYPE_VEC4: return gl::GL_RGBA32F;

        default:
            Fail(
                std::string("Buffer type ") + bufferType.name() +
                " can't be bound to Float Buffer Texture");
        }
    }
    else if (interface.type() == BufferTextureType::Int)
    {
        switch (bufferType.id())
        {
        case TYPE_I8: return gl::GL_R8I;
        case TYPE_I16: return gl::GL_R16I;
        case TYPE_I32: return gl::GL_R32I;
        case TYPE_I8VEC2: return gl::GL_RG8I;
        case TYPE_I16VEC2: return gl::GL_RG16I;
        case TYPE_I32VEC2: return gl::GL_RG32I;
        case TYPE_I8VEC4: return gl::GL_RGBA8I;
        case TYPE_I16VEC4: return gl::GL_RGBA16I;
        case TYPE_I32VEC4: return gl::GL_RGBA32I;

        default:
            Fail(
                std::string("Buffer type ") + bufferType.name() +
                " can't be bound to Int Buffer Texture");
        }
    }
    else if (interface.type() == BufferTextureType::UnsignedInt)
    {
        switch (bufferType.id())
        {
        case TYPE_U8: return gl::GL_R8UI;
        case TYPE_U16: return gl::GL_R16UI;
        case TYPE_U32: return gl::GL_R32UI;
        case TYPE_U8VEC2: return gl::GL_RG8UI;
        case TYPE_U16VEC2: return gl::GL_RG16UI;
        case TYPE_U32VEC2: return gl::GL_RG32UI;
        case TYPE_U8VEC4: return gl::GL_RGBA8UI;
        case TYPE_U16VEC4: return gl::GL_RGBA16UI;
        case TYPE_U32VEC4: return gl::GL_RGBA32UI;

        default:
            Fail(
                std::string("Buffer type ") + bufferType.name() +
                " can't be bound to Int Buffer Texture");
        }
    }

    Fail("");
    return gl::GL_NONE;
}
}