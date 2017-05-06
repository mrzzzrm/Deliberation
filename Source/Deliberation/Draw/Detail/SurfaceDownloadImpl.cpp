#include "SurfaceDownloadImpl.h"

#include <iostream>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Surface.h>

#include "TextureImpl.h"

namespace deliberation
{

SurfaceDownloadImpl::SurfaceDownloadImpl(const Surface & surface):
    surface(surface),
    width(0),
    height(0),
    format(surface.format()),
    glName(0),
    finished(false),
    started(false),
    size(0)
{

}

bool SurfaceDownloadImpl::isDone() const
{
    if (!started)
    {
        return false;
    }

    if (finished)
    {
        return true;
    }

    auto r = gl::glClientWaitSync(sync, gl::GL_NONE_BIT, 0u);
    if (r == gl::GL_WAIT_FAILED)
    {
        std::cout << "Wait for sync failed. This shouldn't happen." << std::endl;
        return false;
    }

    finished = r == gl::GL_ALREADY_SIGNALED || r == gl::GL_CONDITION_SATISFIED;

    return finished;
}

void SurfaceDownloadImpl::start()
{
    Assert(!started, "");

    auto & drawContext = surface.m_impl->textureImpl->drawContext;

    size = surface.width() * surface.height() * format.bytesPerPixel();

    gl::glGenBuffers(1, &glName);
    Assert(glName != 0, "");

    drawContext.m_glStateManager.bindBuffer(gl::GL_PIXEL_PACK_BUFFER, glName);
    gl::glBufferData(gl::GL_PIXEL_PACK_BUFFER, size, nullptr, gl::GL_STREAM_READ);


    const auto textureType = (gl::GLenum)surface.m_impl->textureImpl->type;
    drawContext.m_glStateManager.bindTexture(textureType, surface.m_impl->textureImpl->glName);
    gl::glGetTexImage(textureType,
                      0,
                      format.glFormat(),
                      format.glType(),
                      nullptr);

    sync = gl::glFenceSync(gl::GL_SYNC_GPU_COMMANDS_COMPLETE, (gl::UnusedMask)0);

    drawContext.m_glStateManager.bindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    started = true;
}

const SurfaceBinary & SurfaceDownloadImpl::result() const
{
    Assert(started, "");

    if (surfaceBinary.engaged())
    {
        return surfaceBinary.get();
    }

    if (!isDone())
    {
        while (true)
        {
            auto r = gl::glClientWaitSync(sync, gl::GL_SYNC_FLUSH_COMMANDS_BIT, 1000 * 1000u);
            if (r == gl::GL_WAIT_FAILED)
            {
                Fail("Wait for sync failed. This shouldn't happen.");
            }

            if (r == gl::GL_ALREADY_SIGNALED || r == gl::GL_CONDITION_SATISFIED)
            {
                break;
            }
        }
    }

    auto & drawContext = surface.m_impl->textureImpl->drawContext;
    auto numValues = surface.width() * surface.height() * surface.format().componentsPerPixel();
    auto type = format.glType();

    drawContext.m_glStateManager.bindBuffer(gl::GL_PIXEL_PACK_BUFFER, glName);

    switch (type)
    {
        case gl::GL_BYTE:
            fillSurfaceBinary<gl::GLbyte>(numValues);
            break;
        case gl::GL_UNSIGNED_BYTE:
            fillSurfaceBinary<gl::GLubyte>(numValues);
            break;
        case gl::GL_SHORT:
            fillSurfaceBinary<gl::GLshort>(numValues);
            break;
        case gl::GL_UNSIGNED_SHORT:
            fillSurfaceBinary<gl::GLushort>(numValues);
            break;
        case gl::GL_INT:
            fillSurfaceBinary<gl::GLint>(numValues);
            break;
        case gl::GL_UNSIGNED_INT:
            fillSurfaceBinary<gl::GLuint>(numValues);
            break;
        case gl::GL_FLOAT:
            fillSurfaceBinary<gl::GLfloat>(numValues);
            break;
        default:
            Fail("Unknown format");
    }

    drawContext.m_glStateManager.bindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    return surfaceBinary.get();
}

template<typename T>
void SurfaceDownloadImpl::fillSurfaceBinary(std::size_t numValues) const
{
    std::vector<T> data(numValues);
    gl::glGetBufferSubData(gl::GL_PIXEL_PACK_BUFFER, 0, size, data.data());
    surfaceBinary.reset(std::move(data), surface.width(), surface.height(), surface.format());
}

}

