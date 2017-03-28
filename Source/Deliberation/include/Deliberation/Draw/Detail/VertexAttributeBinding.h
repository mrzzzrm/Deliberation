#pragma once

#include <memory>

#include <Deliberation/Core/Experimental.h>
#include <Deliberation/Deliberation.h>

namespace deliberation
{

namespace detail
{

class BufferImpl;

struct VertexAttributeValueBinding {
    size_t offset = 0;
    size_t attributeIndex = 0;
};

struct VertexAttributeBufferBinding {
    std::shared_ptr <BufferImpl> buffer;
    u32 fieldIndex = 0;

    bool ranged = false;
    u32 first = 0;
    u32 count = 0;
    u32 divisor = 0;
};

using VertexAttributeBinding = std::experimental::variant<
    std::experimental::monostate,
    VertexAttributeValueBinding,
    VertexAttributeBufferBinding>;

}
}