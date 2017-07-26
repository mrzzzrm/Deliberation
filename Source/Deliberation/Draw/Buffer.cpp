#include <Deliberation/Draw/Buffer.h>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Detail/ViewBlobImpl.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include "Detail/BufferImpl.h"
#include "DrawContext.h"

namespace deliberation
{
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
    return "Layout: " + layout().toString() +
           "\n"
           "Count: " +
           std::to_string(count());
}

Buffer::Buffer(const std::shared_ptr<BufferImpl> & impl) : Base(impl) {}

void Buffer::upload(const Blob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(data.size() % layout().stride() == 0, "");

    rawUpload(data, data.size() / layout().stride());
}

void Buffer::rawUpload(const Blob & data, size_t count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->glName != 0, "");

    const auto glTarget = gl::GL_COPY_WRITE_BUFFER;

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(glTarget, m_impl->glName);
    gl::glBufferData(glTarget, data.size(), data.ptr(), gl::GL_STATIC_DRAW);

    m_impl->count = count;
}

void Buffer::upload(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(layout().equals(data.layout()), "Layout mismatch");

    rawUpload(data.rawData(), data.count());
}

void Buffer::reinit(size_t count)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->glName != 0, "Buffer not backed by GL object");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(gl::GL_COPY_WRITE_BUFFER, m_impl->glName);
    gl::glBufferData(
        gl::GL_COPY_WRITE_BUFFER,
        count * m_impl->layout.stride(),
        NULL,
        gl::GL_STREAM_DRAW);

    m_impl->count = count;
}

void Buffer::mapped(
    BufferMapping flags, const std::function<void(LayoutedBlob & mapping)> & fn)
{
    Assert(m_impl.get(), "Can't perform action on hollow object");
    Assert(m_impl->glName != 0, "Buffer not backed by GL object");
    Assert(m_impl->count != 0, "Can't map empty buffer");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;
    glStateManager.bindBuffer(gl::GL_COPY_WRITE_BUFFER, m_impl->glName);

    gl::GLenum access;
    switch (flags)
    {
    case BufferMapping::ReadOnly: access = gl::GL_READ_ONLY; break;
    case BufferMapping::WriteOnly: access = gl::GL_WRITE_ONLY; break;
    case BufferMapping::ReadWrite: access = gl::GL_READ_WRITE; break;
    }

    do
    {
        auto * mapped = gl::glMapBuffer(gl::GL_COPY_WRITE_BUFFER, access);
        Assert(mapped, "Failed to map buffer");

        auto viewBlob = std::make_unique<detail::ViewBlobImpl>(mapped, size());

        auto mappedBlob =
            LayoutedBlob(m_impl->layout, Blob(std::move(viewBlob)));
        fn(mappedBlob);
    } while (gl::glUnmapBuffer(gl::GL_COPY_WRITE_BUFFER) == gl::GL_FALSE);
}
}
