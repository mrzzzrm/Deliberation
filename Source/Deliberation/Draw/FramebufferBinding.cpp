#include <Deliberation/Draw/FramebufferBinding.h>

#include <Deliberation/Draw/Detail/FramebufferImpl.h>

#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{
const char * const FramebufferBinding::BACKBUFFER = "<Backbuffer>";
const char * const FramebufferBinding::DISCARD_FRAGMENT_OUTPUT = "";

FramebufferBinding::FramebufferBinding(
    const Program & program,
    Framebuffer &                                       framebuffer,
    const FramebufferMappings &                         mappings)
    : m_program(program)
    , m_framebuffer(framebuffer)
    , m_mappings(mappings)
{
}

void FramebufferBinding::setFramebuffer(Framebuffer & framebuffer)
{
    AssertM(m_program.isEngaged(), "FramebufferBinding is hollow");

    m_framebuffer = framebuffer;
    m_drawBuffersDirty = true;
}

void FramebufferBinding::setMappings(const FramebufferMappings & mappings)
{
    AssertM(m_program.isEngaged(), "FramebufferBinding is hollow");

    m_mappings = mappings;
    m_drawBuffersDirty = true;
}

void FramebufferBinding::setMapping(const FragmentOutputMapping & mapping)
{
    AssertM(m_program.isEngaged(), "FramebufferBinding is hollow");

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
    AssertM(m_program.isEngaged(), "FramebufferBinding is hollow");

    if (m_drawBuffersDirty)
    {
        if (m_framebuffer.isBackbuffer())
        {
            /**
             * If we have just one output, everything is alright.
             * Otherwise there has to be precisely one output being mapped to something different than
             * DISCARD_FRAGMENT_OUTPUT
             */
            if (fragmentOutputs().size() != 1)
            {
                m_drawBuffers = std::vector<gl::GLenum>(fragmentOutputs().size(), gl::GL_NONE);
                auto foundOutput = false;
                for (size_t o = 0; o < fragmentOutputs().size(); o++)
                {
                    auto & fragmentOutput = fragmentOutputs()[o];

                    // Does the output have a mapping?
                    auto iter = std::find_if(
                        m_mappings.begin(),
                        m_mappings.end(),
                        [&](const FragmentOutputMapping & fragmentOutputMapping) {
                            return "o_" + fragmentOutputMapping.first ==
                                   fragmentOutput.name();
                        });

                    if (iter != m_mappings.end() && iter->second != DISCARD_FRAGMENT_OUTPUT)
                    {
                        AssertM(!foundOutput,
                               "Multiple outputs in draw to backbuffer");
                        foundOutput = true;
                        m_drawBuffers[fragmentOutput.location()] = gl::GL_BACK_LEFT;
                    }
                }
                Assert(foundOutput);
            }
        }
        else
        {
            auto & colorTargets = m_framebuffer.m_impl->colorTargets;

            std::vector<gl::GLenum> bufs(
                fragmentOutputs().size(), gl::GL_NONE);
            for (size_t o = 0; o < fragmentOutputs().size(); o++)
            {
                auto & fragmentOutput = fragmentOutputs()[o];

                std::string mappedTargetName;

                // Does the output have a mapping?
                auto iter = std::find_if(
                    m_mappings.begin(),
                    m_mappings.end(),
                    [&](const FragmentOutputMapping & fragmentOutputMapping) {
                        return "o_" + fragmentOutputMapping.first ==
                               fragmentOutput.name();
                    });


                if (iter != m_mappings.end())
                {
                    /**
                     * \ref bufs gets initialised with GL_NONE, which means: discard the output
                     */
                    if (iter->second == DISCARD_FRAGMENT_OUTPUT) continue;

                    mappedTargetName = "o_" + iter->second;
                }

                for (size_t t = 0; t < colorTargets.size(); t++)
                {
                    auto &     colorTarget = colorTargets[t];
                    const auto colorTargetName = "o_" + colorTarget.name;

                    // If no mapping exist, mappedTargetName will be empty and
                    // never match
                    if (colorTargetName == mappedTargetName ||
                        "o_" + colorTarget.name == fragmentOutput.name())
                    {
                        /**
                         * This check is not correct: It is indeed logical to
                         * draw from RGBA to RGB, when, e.g., blending
                         */
                        //                        AssertM(
                        //                            colorTarget.surface.format().fragmentOutputType()
                        //                            == fragmentOutput.type(),
                        //                            "Fragment output " +
                        //                            fragmentOutput.name() +
                        //                            "(" +
                        //                            fragmentOutput.type().name()
                        //                            +
                        //                            ") and RenderTarget (" +
                        //                            colorTarget.surface.format().toString()
                        //                            + " = " +
                        //                            colorTarget.surface.format()
                        //                                .fragmentOutputType()
                        //                                .name() +
                        //                            ") are incompatible");

                        bufs[fragmentOutput.location()] =
                            (gl::GLenum)((u32)gl::GL_COLOR_ATTACHMENT0 + t);
                        break;
                    }
                }

                AssertM(
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