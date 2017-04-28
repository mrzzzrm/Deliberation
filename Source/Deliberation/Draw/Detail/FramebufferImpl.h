#pragma once

#include <memory>
#include <vector>



#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

class DrawContext;
class GLFramebuffer;
class GLStateManager;
class Surface;

namespace detail
{

/*
    TODO
        Members should be public
*/

class FramebufferImpl final
{
public:
    static std::shared_ptr<FramebufferImpl> backbuffer(DrawContext & drawContext,
                                                       unsigned int width,
                                                       unsigned int height);
    static std::shared_ptr<FramebufferImpl> custom(DrawContext & drawContext,
                                                   unsigned int width,
                                                   unsigned int height);

public:
    FramebufferImpl(DrawContext & drawContext);

    DrawContext & drawContext() const;

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    Surface * renderTarget(unsigned int index);
    const Surface * renderTarget(unsigned int index) const;

    const std::vector<Optional<Surface>> & renderTargets() const;

    Surface * depthTarget();
    const Surface * depthTarget() const;

    void setRenderTarget(unsigned int index, Surface * surface);

    void setDepthTarget(Surface * surface);

    void addRenderTarget(PixelFormat format, int index = -1);
    void addDepthTarget(PixelFormat format);

    void bind(GLStateManager & glStateManager) const;

public:
    DrawContext &                               m_drawContext;
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

    mutable Optional<Clear>                     m_clear;

private:
    void updateFramebufferDesc() const;
};

}

}

