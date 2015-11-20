#pragma once

#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/OffsetOf.h>

#include <Deliberation/Draw/GL/GLSLTypeOf.h>
#include <Deliberation/Draw/Detail/NamedDataMemberOf.h>
#include <Deliberation/Draw/BufferLayoutField.h>

namespace deliberation
{

class DELIBERATION_API BufferLayout final
{
public:
    BufferLayout();

    bool empty() const;

    size_t stride() const;

    void setStride(size_t stride);

    bool hasField(const std::string & name) const;

    const BufferLayoutField & field(const std::string & name) const;

    const std::vector<BufferLayoutField> & fields() const;

    std::string toString() const;

private:
    friend class Context;

private:
    BufferLayout(const std::vector<BufferLayoutField> & fields);
    BufferLayout(const std::vector<BufferLayoutField> & fields, size_t stride);

private:
    size_t m_stride;
    std::vector<BufferLayoutField> m_fields;
};

}

