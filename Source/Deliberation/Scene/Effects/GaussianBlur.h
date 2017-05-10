#pragma once

#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class Surface;

class GaussianBlur final
{
public:
    GaussianBlur() = default;
    GaussianBlur(DrawContext & drawContext, Surface & inputSurface);

    void render();

    Framebuffer & output() { return m_outputBuffer; }

private:
    bool                m_initialized = false;
    ScreenSpaceEffect   m_horizontalBlur;
    ScreenSpaceEffect   m_verticalBlur;
    Framebuffer         m_intermediateBuffer;
    Framebuffer         m_outputBuffer;
};

}