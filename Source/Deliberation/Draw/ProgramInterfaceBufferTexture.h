#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{
enum class BufferTextureType
{
    Float,
    UnsignedInt,
    Int
};

class ProgramInterfaceBufferTexture
{
public:
    ProgramInterfaceBufferTexture(
        const std::string & name,
        BufferTextureType   type,
        u32                 index,
        gl::GLuint          location)
        : m_name(name), m_type(type), m_index(index), m_location(location)
    {
    }

    const std::string &       name() const { return m_name; }
    const BufferTextureType & type() const { return m_type; }
    u32                       index() const { return m_index; }
    const gl::GLuint &        location() const { return m_location; }

    std::string toString() const;

private:
    std::string       m_name;
    BufferTextureType m_type;
    uint32_t          m_index;
    gl::GLuint        m_location;
};
}