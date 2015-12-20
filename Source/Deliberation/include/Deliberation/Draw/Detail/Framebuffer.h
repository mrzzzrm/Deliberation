#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

namespace deliberation
{

class GLFramebuffer;
class GLStateManager;
class Surface;

namespace detail
{

class DELIBERATION_API Framebuffer final
{
public:
    Framebuffer();

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * surface(unsigned int index) const;
    const std::vector<Surface*> & surfaces() const;

    void setSurface(unsigned int index, Surface * surface);

    void bind(GLStateManager & glStateManager) const;

private:
    void updateFramebufferDesc() const;
    void updateResolution() const;

private:
    bool                                    m_isBackbuffer;
    std::vector<Surface*>                   m_surfaces;
    mutable Optional<GLFramebufferDesc>     m_glFramebufferDesc;
    mutable bool                            m_glFramebufferDirty;
    mutable std::shared_ptr<GLFramebuffer>  m_glFramebuffer;
    mutable unsigned int                    m_width;
    mutable unsigned int                    m_height;
    mutable bool                            m_resolutionDirty;
};

}

}

