#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Draw/BufferLayout.h>

/*
    TODO
        Create buffers with immutable size only?
*/

namespace deliberation
{

namespace detail
{
    class BufferImpl;
}

class Context;

class Buffer final
{
public:
    unsigned int count() const;
    const BufferLayout & layout() const;

    std::string toString() const;

private:
    friend class Draw;
    friend class GLVertexAttributeBinder;
    friend class BufferUploadExecution;
    friend class Context;

private:
    Buffer(Context & context, const BufferLayout & layout);

private:
    std::shared_ptr<detail::BufferImpl> m_impl;
};

}

