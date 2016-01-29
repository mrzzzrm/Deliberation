#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>
#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

class GLFramebuffer;
class GLStateManager;
class Surface;

namespace detail
{

class DELIBERATION_API FramebufferImpl final
{
public:
    static std::shared_ptr<FramebufferImpl> backbuffer(unsigned int width, unsigned int height);

public:
    FramebufferImpl();

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Optional<Surface>> & renderTargets() const;

    Surface * depthTarget() const;

    void setRenderTarget(unsigned int index, Surface * surface);

    void setDepthTarget(Surface * surface);

    void bind(GLStateManager & glStateManager) const;

private:
    void updateFramebufferDesc() const;
    void updateResolution() const;

private:
    bool                                    m_isBackbuffer;
    std::vector<Optional<Surface>>          m_renderTargets;
    mutable Optional<Surface>               m_depthTarget;
    mutable Optional<GLFramebufferDesc>     m_glFramebufferDesc;
    mutable bool                            m_glFramebufferDirty;
    mutable std::shared_ptr<GLFramebuffer>  m_glFramebuffer;
    mutable unsigned int                    m_width;
    mutable unsigned int                    m_height;
    mutable bool                            m_resolutionDirty;
};

}

}

