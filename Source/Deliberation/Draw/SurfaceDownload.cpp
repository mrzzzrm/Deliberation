#include <Deliberation/Draw/SurfaceDownload.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/SurfaceDownloadImpl.h"

namespace deliberation
{
SurfaceDownload::SurfaceDownload() {}

bool SurfaceDownload::isDone() const
{
    Assert(m_impl.get(), "SurfaceDownload is hollow");
    return m_impl->isDone();
}

void SurfaceDownload::start()
{
    Assert(m_impl.get(), "SurfaceDownload is hollow");
    m_impl->start();
}

const SurfaceBinary & SurfaceDownload::result() const
{
    Assert(m_impl.get(), "SurfaceDownload is hollow");
    return m_impl->result();
}

SurfaceDownload::SurfaceDownload(
    const std::shared_ptr<SurfaceDownloadImpl> & impl)
    : m_impl(impl)
{
}
}
