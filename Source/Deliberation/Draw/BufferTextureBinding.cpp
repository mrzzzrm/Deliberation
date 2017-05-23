#include <Deliberation/Draw/BufferTextureBinding.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Detail/DrawImpl.h>
#include <Deliberation/Draw/Detail/ProgramImpl.h>

namespace deliberation
{

void BufferTextureBinding::setBuffer(const Buffer & buffer)
{
    setBufferRange(buffer, 0, buffer.count());
}

void BufferTextureBinding::setBufferRange(const Buffer & buffer, u32 begin, u32 count)
{
    const auto & interface = m_drawImpl->program->interface.bufferTextures()[m_index];

    Assert(buffer.layout().fields().size() == 1, "Buffer Textures are only allowed to have one field");

    const auto bufferType = buffer.layout().field(0).type();

    gl::GLenum internalFormat = gl::GL_NONE;

    if (interface.type() == BufferTextureType::Float)
    {
        switch (bufferType.id())
        {
            case TYPE_U8: internalFormat = gl::GL_R8; break;
            case TYPE_U16: internalFormat = gl::GL_R16; break;
            case TYPE_U8VEC2: internalFormat = gl::GL_RG8; break;
            case TYPE_U16VEC2: internalFormat = gl::GL_RG16; break;
            case TYPE_U8VEC4: internalFormat = gl::GL_RGBA8; break;
            case TYPE_U16VEC4: internalFormat = gl::GL_RGBA16; break;
            case TYPE_FLOAT: internalFormat = gl::GL_R32F; break;
            case TYPE_VEC2: internalFormat = gl::GL_RG32F; break;
            case TYPE_VEC3: internalFormat = gl::GL_RGB32F; break;
            case TYPE_VEC4: internalFormat = gl::GL_RGBA32F; break;

            default:
                Fail(std::string("Buffer type ") + bufferType.name() + " can't be bound to Float Buffer Texture");
        }
    }
    else if (interface.type() == BufferTextureType::Int)
    {
        switch (bufferType.id())
        {
            case TYPE_I8: internalFormat = gl::GL_R8I; break;
            case TYPE_I16: internalFormat = gl::GL_R16I; break;
            case TYPE_I32: internalFormat = gl::GL_R32I; break;
            case TYPE_I8VEC2: internalFormat = gl::GL_RG8I; break;
            case TYPE_I16VEC2: internalFormat = gl::GL_RG16I; break;
            case TYPE_I32VEC2: internalFormat = gl::GL_RG32I; break;
            case TYPE_I8VEC4: internalFormat = gl::GL_RGBA8I; break;
            case TYPE_I16VEC4: internalFormat = gl::GL_RGBA16I; break;
            case TYPE_I32VEC4: internalFormat = gl::GL_RGBA32I; break;

            default:
            Fail(std::string("Buffer type ") + bufferType.name() + " can't be bound to Int Buffer Texture");
        }
    }
    else if (interface.type() == BufferTextureType::UnsignedInt)
    {
        switch (bufferType.id())
        {
            case TYPE_U8: internalFormat = gl::GL_R8UI; break;
            case TYPE_U16: internalFormat = gl::GL_R16UI; break;
            case TYPE_U32: internalFormat = gl::GL_R32UI; break;
            case TYPE_U8VEC2: internalFormat = gl::GL_RG8UI; break;
            case TYPE_U16VEC2: internalFormat = gl::GL_RG16UI; break;
            case TYPE_U32VEC2: internalFormat = gl::GL_RG32UI; break;
            case TYPE_U8VEC4: internalFormat = gl::GL_RGBA8UI; break;
            case TYPE_U16VEC4: internalFormat = gl::GL_RGBA16UI; break;
            case TYPE_U32VEC4: internalFormat = gl::GL_RGBA32UI; break;

            default:
            Fail(std::string("Buffer type ") + bufferType.name() + " can't be bound to Int Buffer Texture");
        }
    }

    m_drawImpl->drawContext.m_glStateManager.bindTexture(gl::GL_TEXTURE_BUFFER,
        m_drawImpl->bufferTextures[m_index]);
    gl::glTexBufferRange(gl::GL_TEXTURE_BUFFER, buffer.m_impl->glName);
}

}