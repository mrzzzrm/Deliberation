#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

class Context;
class GLFramebuffer;
class GLStateManager;
class Surface;

namespace detail
{

/*
    TODO
        Members should be public
*/

class DELIBERATION_API FramebufferImpl final
{
public:
    static std::shared_ptr<FramebufferImpl> backbuffer(Context & context,
                                                       unsigned int width,
                                                       unsigned int height);
    static std::shared_ptr<FramebufferImpl> custom(Context & context,
                                                   unsigned int width,
                                                   unsigned int height);

public:
    FramebufferImpl(Context & context);

    Context & context() const;

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Optional<Surface>> & renderTargets() const;

    Surface * depthTarget() const;

    void setRenderTarget(unsigned int index, Surface * surface);

    void setDepthTarget(Surface * surface);

    void addRenderTarget(PixelFormat format, int index = -1);
    void addDepthTarget(PixelFormat format);

    Clear createClear();

    void bind(GLStateManager & glStateManager) const;

private:
    void updateFramebufferDesc() const;
    void updateResolution() const;

private:
    Context &                                   m_context;
    bool                                        m_isBackbuffer;

    // For RenderTargets created by the Framebuffer (via addRenderTarget(format), e.g.)
    LinearMap<Texture>                          m_renderTargetTextures;
    Optional<Texture>                           m_depthTargetTexture;

    std::vector<Optional<Surface>>              m_renderTargets;
    mutable Optional<Surface>                   m_depthTarget;
    mutable Optional<GLFramebufferDesc>         m_glFramebufferDesc;
    mutable bool                                m_glFramebufferDirty;
    mutable std::shared_ptr<GLFramebuffer>      m_glFramebuffer;
    mutable unsigned int                        m_width;
    mutable unsigned int                        m_height;
    mutable bool                                m_resolutionDirty;
};

}

}

