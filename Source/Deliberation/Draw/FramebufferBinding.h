#pragma once

#include <vector>

#include <glbinding/gl/enum.h>

#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
using FragmentOutputMapping = std::pair<std::string, std::string>;
using FramebufferMappings = std::vector<FragmentOutputMapping>;

/**
 * Describes the way a Draw should write to a Framebuffer
 * Use Draw::framebufferBinding(Framebuffer &, const FramebufferMappings &) to create one
 *
 * By the default the Program's Fragment Output o_<Name> will be written to the Framebuffer's Color Target <Name>.
 *
 * FramebufferBinding::setMappings(const FramebufferMappings&) /
 * FramebufferBinding::setMapping(const FragmentOutputMapping &) can be used to override this. They will however
 * not check for the validity of the mapping (such as whether fragment outputs with the given names exist).
 * Error checking will happen when the Draw calls FramebufferBinding::drawBuffersGL()
 */
class FramebufferBinding final
{
public:
    static const char * const DISCARD_FRAGMENT_OUTPUT;

public:
    FramebufferBinding() = default;
    FramebufferBinding(
        const Program & program,
        Framebuffer &                                       framebuffer,
        const FramebufferMappings &                         mappings);

    Framebuffer &       framebuffer() { return m_framebuffer; }
    const Framebuffer & framebuffer() const { return m_framebuffer; }

    void setFramebuffer(Framebuffer & framebuffer);
    void setMappings(const FramebufferMappings & mappings);

    /**
     * Update a single fragment output <-> render target relation
     * Does NOT clear the other mappings, use setMappings({}) for this
     * @param mapping
     */
    void setMapping(const FragmentOutputMapping & mapping);

    const std::vector<gl::GLenum> & drawBuffersGL() const;

private:
    const std::vector<ProgramInterfaceFragmentOutput> & fragmentOutputs() const { return m_program.interface().fragmentOutputs(); }

private:
    Program             m_program;
    Framebuffer         m_framebuffer;
    FramebufferMappings m_mappings;

    mutable bool                    m_drawBuffersDirty = true;
    mutable std::vector<gl::GLenum> m_drawBuffers;
};
}