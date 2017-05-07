#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/SurfaceBinary.h>

namespace deliberation
{
class SurfaceDownloadImpl
{
  public:
    SurfaceDownloadImpl(const Surface & surface);

    bool isDone() const;

    void start();

    const SurfaceBinary & result() const;

    template<typename T>
    void fillSurfaceBinary(std::size_t numValues) const;

    Surface                         surface;
    unsigned int                    width;
    unsigned int                    height;
    PixelFormat                     format;
    gl::GLuint                      glName;
    gl::GLsync                      sync;
    mutable Optional<SurfaceBinary> surfaceBinary;
    mutable bool                    finished;
    mutable bool                    started;
    std::size_t                     size;
};
}
