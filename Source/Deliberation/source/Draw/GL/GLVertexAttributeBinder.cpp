#include "GLVertexAttributeBinder.h"

#include <cassert>
#include <iostream>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLType.h>

#include "Draw/Detail/BufferImpl.h"

#include "GLVertexAttributeFormat.h"

namespace deliberation
{

GLVertexAttributeBinder::GLVertexAttributeBinder(gl::GLuint vao,
                                                 const ProgramInterface & programInterface,
                                                 const detail::BufferImpl & buffer,
                                                 gl::GLuint divisor):
    m_vao(vao),
    m_programInterface(programInterface),
    m_buffer(buffer),
    m_divisor(divisor)
{
}

void GLVertexAttributeBinder::bind(const std::string & name, gl::GLint baseoffset)
{
    auto programAttribute = m_programInterface.attribute(name);
    auto bufferField = m_buffer.layout.field(name);

    if (programAttribute.type() != GLType(bufferField.type()))
    {
        std::cout << "Cannot bind '" << bufferField.name() << "' of type " << bufferField.type().name() << " to vertex attribute of type " << programAttribute.type() << std::endl;
        assert(false);
    }

    auto format = GLVertexAttributeFormat(programAttribute.type());

    auto startLocation = programAttribute.location();
    assert(startLocation >= 0);

    /*
        TODO
            gl calls - out of here!?
    */

    gl::glBindVertexArray(m_vao);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_buffer.glName);

    Assert(format.numLocations() == 1, "Implement this");

    for (decltype(startLocation) l = startLocation; l < startLocation + format.numLocations(); l++)
    {
        gl::glEnableVertexAttribArray(l);
        gl::glVertexAttribDivisor(l, m_divisor);

        auto localLocation = l - startLocation;
        auto offset = bufferField.offset() + format.relativeOffsetOfLocation(localLocation);

        switch (format.call())
        {
        case GLVertexAttributeFormatType::Format:
            gl::glVertexAttribPointer(l, format.size(), format.type(), gl::GL_FALSE, m_buffer.layout.stride(), (const gl::GLvoid*)(baseoffset + offset));
//            binding->setFormat(format.size(), format.type(), gl::GL_FALSE, offset);
            break;
        case GLVertexAttributeFormatType::IFormat:
            gl::glVertexAttribIPointer(l, format.size(), format.type(), m_buffer.layout.stride(), (const gl::GLvoid*)(baseoffset + offset));
//            binding->setIFormat(format.size(), format.type(), offset);
            break;
//        case VertexAttributeFormatCall::LFormat:
//            binding->setLFormat(format.size(), format.type(), offset);
//            break;
        }
    }
}

void GLVertexAttributeBinder::bindAll(gl::GLint baseoffset)
{
    for (auto & field : m_buffer.layout.fields())
    {
        bind(field.name(), baseoffset);
    }
}

}


