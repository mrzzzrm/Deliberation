#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class GaussianBlur;

class GaussianBlurPass
{
public:
    GaussianBlurPass(
        GaussianBlur & gaussianBlur,
        Framebuffer & lFb,
        Framebuffer & rFb);

    u32 radius() const;

    void setStandardDeviation(float s);

    void render();

private:
    GaussianBlur &  m_gaussianBlur;
    Framebuffer     m_rFb;
    Surface         m_rSurface;
    Framebuffer     m_lFb;
    Surface         m_lSurface;
    Buffer          m_configBuffer;
    u32             m_radius = 0;
    DataLayoutField m_offsetsField;
    DataLayoutField m_weightsField;
    DataLayoutField m_numSamplesField;
};

}