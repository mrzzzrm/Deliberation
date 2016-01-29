#pragma once

#include <memory>
#include <vector>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class ClearImpl;
}

class Framebuffer;

class DELIBERATION_API Clear final
{
public:
    Clear();

    Framebuffer & framebuffer();
    const Framebuffer & framebuffer() const;

    void setFramebuffer(Framebuffer & framebuffer);

    void setColor(const glm::vec4 & color);
    void disableColor();

    void setDepth(float depth);
    void disableDepth();

    void setStencil(gl::GLint stencil);
    void disableStencil();

    void schedule();

private:
    friend class Context;
    friend class ClearExecution;

private:
    Clear(const std::shared_ptr<detail::ClearImpl> & impl);

private:
    std::shared_ptr<detail::ClearImpl> m_impl;
};

}

