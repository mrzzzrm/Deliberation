#include <Deliberation/Draw/FramebufferBinding.h>

#include <Deliberation/Draw/Detail/FramebufferImpl.h>

#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{

FramebufferBinding::FramebufferBinding(const std::vector<ProgramInterfaceFragmentOutput> & m_fragmentOutputs,
                                       Framebuffer & framebuffer,
                                       const FramebufferMappings & mappings):
    m_fragmentOutputs(&m_fragmentOutputs),
    m_framebuffer(framebuffer),
    m_mappings(mappings)
{

}

void FramebufferBinding::setFramebuffer(Framebuffer & framebuffer)
{
    Assert(m_fragmentOutputs != nullptr, "FramebufferBinding is hollow");

    m_framebuffer = framebuffer;
    m_drawBuffersDirty = true;
}

void FramebufferBinding::setMappings(const FramebufferMappings & mappings)
{
    Assert(m_fragmentOutputs != nullptr, "FramebufferBinding is hollow");

    m_mappings = mappings;
    m_drawBuffersDirty = true;
}

void FramebufferBinding::setMapping(const FragmentOutputMapping & mapping)
{
    Assert(m_fragmentOutputs != nullptr, "FramebufferBinding is hollow");

    m_drawBuffersDirty = true;

    for (auto & mapping2 : m_mappings)
    {
        if (mapping.first == mapping2.first)
        {
            mapping2.second = mapping.second;
            return;
        }
    }

    m_mappings.emplace_back(mapping);
}

const std::vector<gl::GLenum> & FramebufferBinding::drawBuffersGL() const
{
    Assert(m_fragmentOutputs != nullptr, "FramebufferBinding is hollow");

    if (m_drawBuffersDirty)
    {
        if (m_framebuffer.isBackbuffer())
        {
            Assert(
                m_fragmentOutputs->size() == 1,
                "Writing to multiple targets in backbuffer draw");
        }
        else
        {
            auto & colorTargets = m_framebuffer.m_impl->colorTargets;

            Assert(
                m_fragmentOutputs->size() == colorTargets.size(),
                "Framebuffer/m_fragmentOutputs mismatch");

            std::vector<gl::GLenum> bufs(m_fragmentOutputs->size(), gl::GL_NONE);
            for (size_t o = 0; o < m_fragmentOutputs->size(); o++)
            {
                auto & fragmentOutput = (*m_fragmentOutputs)[o];

                std::string mappedTargetName;

                // Does the output have a mapping?
                auto iter = std::find_if(
                    m_mappings.begin(),
                    m_mappings.end(),
                    [&](const FragmentOutputMapping & fragmentOutputMapping) {
                        return "o_" + fragmentOutputMapping.first ==
                               fragmentOutput.name();
                    });

                if (iter != m_mappings.end()) mappedTargetName = "o_" + iter->second;

                for (size_t t = 0; t < colorTargets.size(); t++)
                {
                    auto &     colorTarget = colorTargets[t];
                    const auto colorTargetName = "o_" + colorTarget.name;

                    // If no mapping exist, mappedTargetName will be empty and never
                    // match
                    if (colorTargetName == mappedTargetName ||
                        "o_" + colorTarget.name == fragmentOutput.name())
                    {
                        /**
                         * This check is not correct: It is indeed logical to draw from RGBA to RGB, when, e.g.,
                         * blending
                         */
//                        Assert(
//                            colorTarget.surface.format().fragmentOutputType() ==
//                            fragmentOutput.type(),
//                            "Fragment output " + fragmentOutput.name() + "(" +
//                            fragmentOutput.type().name() +
//                            ") and RenderTarget (" +
//                            colorTarget.surface.format().toString() + " = " +
//                            colorTarget.surface.format()
//                                .fragmentOutputType()
//                                .name() +
//                            ") are incompatible");

                        bufs[fragmentOutput.location()] =
                            (gl::GLenum)((u32)gl::GL_COLOR_ATTACHMENT0 + t);
                        break;
                    }
                }

                Assert(
                    bufs[fragmentOutput.location()] != gl::GL_NONE,
                    "No matching target in Framebuffer for fragment output '" +
                    fragmentOutput.name() + "'");
            }
            m_drawBuffers = std::move(bufs);
        }
        
        m_drawBuffersDirty = false;
    }

    return m_drawBuffers;
}
}