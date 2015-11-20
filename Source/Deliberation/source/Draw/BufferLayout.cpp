#include <Deliberation/Draw/BufferLayout.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include <glbinding/Meta.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLSLSizeOf.h>

namespace deliberation
{

BufferLayout::BufferLayout():
    m_stride(0)
{

}

BufferLayout::BufferLayout(const std::vector<BufferLayoutField> & fields):
    m_fields(fields),
    m_stride(0)
{
    std::unordered_set<std::string> uniquenessCheck;

    for (auto & field : fields)
    {
        Assert(uniquenessCheck.find(field.name()) == uniquenessCheck.end(),
               "Duplicate field '" + field.name() + "'");

        m_stride += GLSLSizeOf(field.type());
        uniquenessCheck.insert(field.name());
    }
}

BufferLayout::BufferLayout(const std::vector<BufferLayoutField> & fields,
                           size_t stride):
    BufferLayout(fields)
{
    Assert(stride >= m_stride, "");
    m_stride = stride;
}

bool BufferLayout::empty() const
{
    return m_fields.empty();
}

size_t BufferLayout::stride() const
{
    return m_stride;
}

void BufferLayout::setStride(size_t stride)
{
    m_stride = stride;
}

bool BufferLayout::hasField(const std::string & name) const
{
    auto i = std::find_if(m_fields.begin(), m_fields.end(), [&](const BufferLayoutField & field)
    {
        return field.name() == name;
    });
    return i != m_fields.end();
}

const BufferLayoutField & BufferLayout::field(const std::string & name) const
{
    auto i = std::find_if(m_fields.begin(), m_fields.end(), [&](const BufferLayoutField & field)
    {
        return field.name() == name;
    });
    Assert(i != m_fields.end(), "");

    return *i;
}

const std::vector<BufferLayoutField> & BufferLayout::fields() const
{
    return m_fields;
}

std::string BufferLayout::toString() const
{
    std::stringstream stream;
    stream << "Stride: " << m_stride << " Fields: " << m_fields.size() << std::endl;
    for (auto & field : m_fields)
    {
        stream << "    <" << field.name() << ">: Type: " << field.type() << " Offset: " << field.offset() << " Size: " << field.size() << std::endl;
    }
    return stream.str();
}

}


