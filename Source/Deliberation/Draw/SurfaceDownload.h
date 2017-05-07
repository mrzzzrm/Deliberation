#pragma once

#include <Deliberation/Draw/SurfaceBinary.h>

namespace deliberation
{
class DrawContext;
class SurfaceDownloadImpl;

class SurfaceDownload final
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

  private:
    std::shared_ptr<SurfaceDownloadImpl> m_impl;
};
}
