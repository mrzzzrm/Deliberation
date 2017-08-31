#include <Deliberation/Draw/SurfaceDownload.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/SurfaceDownloadImpl.h"

namespace deliberation
{
SurfaceDownload::SurfaceDownload() {}

bool SurfaceDownload::isDone() const
{
    AssertM(m_impl.get(), "SurfaceDownload is hollow");
    return m_impl->isDone();
}

void SurfaceDownload::start()
{
    AssertM(m_impl.get(), "SurfaceDownload is hollow");
    m_impl->start();
}

const SurfaceBinary & SurfaceDownload::result() const
{
    AssertM(m_impl.get(), "SurfaceDownload is hollow");
    return m_impl->result();
}

SurfaceDownload::SurfaceDownload(
    const std::shared_ptr<SurfaceDownloadImpl> & impl)
    : Base(impl)
{
}
}
