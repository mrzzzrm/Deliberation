#include "BufferImpl.h"

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

namespace detail
{

BufferImpl::BufferImpl(Context & context):
    context(context),
    glName(0u),
    count(0u)
{
    context.allocateBuffer(*this);
}

BufferImpl::~BufferImpl()
{
    context.deallocateBuffer(*this);
}

}

}

