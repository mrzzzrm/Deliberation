#pragma once

#include <vector>

#include <glbinding/gl/enum.h>

#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/ProgramInterfaceFragmentOutput.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

using FragmentOutputMapping = std::pair<std::string, std::string>;
using FramebufferMappings = std::vector<FragmentOutputMapping>;

class FramebufferBinding final
{
public:
    FramebufferBinding() = default;
    FramebufferBinding(const std::vector<ProgramInterfaceFragmentOutput> & fragmentOutputs,
                       Framebuffer & framebuffer,
                       const FramebufferMappings & mappings);

    Framebuffer & framebuffer() { return m_framebuffer; }
    const Framebuffer & framebuffer() const { return m_framebuffer; }

    void setFramebuffer(Framebuffer & framebuffer);
    void setMappings(const FramebufferMappings & mappings);
    void setMapping(const FragmentOutputMapping & mapping);

    const std::vector<gl::GLenum> & drawBuffersGL() const;

private:
    const std::vector<ProgramInterfaceFragmentOutput> *
                                            m_fragmentOutputs = nullptr;
    Framebuffer                             m_framebuffer;
    FramebufferMappings                     m_mappings;

    mutable bool                            m_drawBuffersDirty = true;
    mutable std::vector<gl::GLenum>         m_drawBuffers;
};

}