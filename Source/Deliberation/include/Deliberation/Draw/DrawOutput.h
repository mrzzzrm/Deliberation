#pragma once

#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Detail/Framebuffer.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
    class ProgramImpl;
}

class Surface;

class DELIBERATION_API DrawOutput
{
public:
    DrawOutput(const DrawOutput & other);

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Surface*> & renderTargets() const;

    void setRenderTarget(unsigned int index, Surface * surface);
    void setRenderTarget(const std::string & name, Surface * surface);

    void setDepthTarget(Surface * surface);

private:
    friend class detail::DrawExecution;
    friend class detail::DrawImpl;

private:
    DrawOutput(const detail::ProgramImpl & program);

private:
    const detail::ProgramImpl & m_program;
    detail::Framebuffer         m_framebuffer;
};

}

