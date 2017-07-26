#pragma once

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/SurfaceBinary.h>

namespace deliberation
{
class DrawContext;
class SurfaceDownloadImpl;

class SurfaceDownload final:
    public AbstractDrawObject<SurfaceDownloadImpl>
{
public:
    SurfaceDownload();

    bool isDone() const;

    void start();

    const SurfaceBinary & result() const;

private:
    friend class DrawContext;
    friend class Surface;

private:
    SurfaceDownload(const std::shared_ptr<SurfaceDownloadImpl> & impl);
};
}
