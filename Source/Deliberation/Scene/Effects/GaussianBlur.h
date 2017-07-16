#pragma once

#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Sampler.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{
class Surface;

class GaussianBlur final
{
public:
    GaussianBlur(DrawContext & drawContext);

    DrawContext &      drawContext() const { return m_blur.drawContext(); }
    const DataLayout & configBlockLayout() const { return m_configBlockLayout; }
    u32                maxNumSamples() const { return m_maxNumSamples; }

    void setInput(const Surface & surface);
    void setFramebuffer(
        Framebuffer & framebuffer, const FramebufferMappings & mappings = {});
    void setConfigBuffer(const Buffer & buffer);

    void renderHBlur();
    void renderVBlur();

private:
    ScreenSpaceEffect   m_blur;
    Sampler             m_inputSampler;
    Uniform             m_horizontalUniform;
    u32                 m_maxNumSamples;
    UniformBufferHandle m_configBuffer;

    DataLayout m_configBlockLayout;
};
}