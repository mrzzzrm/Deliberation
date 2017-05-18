#pragma once

#include <Deliberation/Draw/Sampler.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class Surface;

class GaussianBlur final
{
public:
    GaussianBlur(DrawContext & drawContext);

    void setInput(const Surface & surface);
    void setFramebuffer(Framebuffer & framebuffer, const FramebufferBinding & fbBinding = {});

    void setStandardDeviation(float std);

    void renderHBlur();
    void renderVBlur();

private:
    ScreenSpaceEffect   m_blur;
    Sampler             m_inputSampler;
    Uniform             m_horizontalUniform;
    Uniform             m_weightsUniform;
    Uniform             m_numSamplesUniform;
    Uniform             m_offsetsUniform;
    u32                 m_maxNumSamples;

    DataLayout          m_configBlockLayout;
};

}