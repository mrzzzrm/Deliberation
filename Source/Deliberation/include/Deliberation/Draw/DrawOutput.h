#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/GL/GLFramebuffer.h>
#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
}

class Surface;

class DELIBERATION_API DrawOutput
{
public:
    DrawOutput();

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Surface*> & renderTargets() const;

    void setRenderTarget(unsigned int index, Surface * surface);

private:
    friend class detail::DrawExecution;

private:
    void updateFramebufferDesc() const;
    void updateResolution() const;

private:
    bool                                    m_isBackbuffer;
    std::vector<Surface*>                   m_renderTargets;
    mutable Optional<GLFramebufferDesc>     m_glFramebufferDesc;
    mutable bool                            m_glFramebufferDirty;
    mutable std::shared_ptr<GLFramebuffer>  m_glFramebuffer;
    mutable unsigned int                    m_width;
    mutable unsigned int                    m_height;
    mutable bool                            m_resolutionDirty;
};

}

