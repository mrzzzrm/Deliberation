#include <Deliberation/Draw/Buffer.h>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Detail/ViewBlobImpl.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include "Detail/BufferImpl.h"
#include "DrawContext.h"

namespace deliberation
{

Buffer::Buffer():
    m_impl(nullptr)
{

}

const DataLayout & Buffer::layout() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return m_impl->layout;
}

size_t Buffer::count() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return m_impl->count;
}

size_t Buffer::size() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return count() * layout().stride();
}

std::string Buffer::toString() const
{
    return
        "Layout: " + layout().toString() + "\n"
        "Count: " + std::to_string(count());
}

Buffer::Buffer(const std::shared_ptr<detail::BufferImpl> & impl):
    m_impl(impl)
{
}

void Buffer::scheduleUpload(const Blob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    BufferUpload(m_impl->drawContext, *this, data).schedule();
}

void Buffer::scheduleRawUpload(const Blob & data, size_t count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    BufferUpload(m_impl->drawContext, *this, data, count).schedule();
}

void Buffer::scheduleUpload(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(layout().equals(data.layout()), "Layout mismatch");

    BufferUpload(m_impl->drawContext, *this, data.rawData(), data.count()).schedule();
}

void Buffer::reinit(size_t count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->glName != 0, "Buffer not backed by GL object");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(gl::GL_COPY_WRITE_BUFFER, m_impl->glName);
    gl::glBufferData(gl::GL_COPY_WRITE_BUFFER,
                     count * m_impl->layout.stride(),
                     NULL,
                     gl::GL_STREAM_DRAW);

    m_impl->count = count;
}

LayoutedBlob & Buffer::map(BufferMapping flags)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->glName != 0, "Buffer not backed by GL object");

    if (m_impl->glName == 0)
    {
        m_impl->drawContext.allocateBuffer(*m_impl);
    }

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(gl::GL_COPY_WRITE_BUFFER, m_impl->glName);

    gl::GLenum access;
    switch(flags)
    {
        case BufferMapping::ReadOnly: access = gl::GL_READ_ONLY; break;
        case BufferMapping::WriteOnly: access = gl::GL_WRITE_ONLY; break;
        case BufferMapping::ReadWrite: access = gl::GL_READ_WRITE; break;
    }

    auto * mapped = gl::glMapBuffer(gl::GL_COPY_WRITE_BUFFER, access);
    Assert(mapped, "Failed to map buffer");

    auto viewBlob = std::make_unique<detail::ViewBlobImpl>(mapped, size());

    m_impl->mapped = true;
    m_impl->mappedBlob = LayoutedBlob(m_impl->layout, Blob(std::move(viewBlob)));

    return m_impl->mappedBlob;
}

void Buffer::unmap()
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->mapped, "Buffer not mapped");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(gl::GL_COPY_WRITE_BUFFER, m_impl->glName);

    auto result = gl::glUnmapBuffer(gl::GL_COPY_WRITE_BUFFER);
    Assert(result == gl::GL_TRUE, "GL be like: Here's my middle finger. Deal with it...")
}

}

