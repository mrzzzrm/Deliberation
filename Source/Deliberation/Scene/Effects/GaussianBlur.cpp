#include <Deliberation/Scene/Effects/GaussianBlur.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

GaussianBlur::GaussianBlur(DrawContext & drawContext, Surface & inputSurface)
{
    auto program = drawContext.createProgram({
                                                 DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                                 DeliberationDataPath("Data/Shaders/GaussianBlur.frag")
                                             });

    m_intermediateBuffer = drawContext.createFramebuffer({inputSurface.width(), inputSurface.height(),
                                                          {{PixelFormat_RGB_32_F, "Blurred"}}});
    m_outputBuffer = drawContext.createFramebuffer({inputSurface.width(), inputSurface.height(),
                                                          {{PixelFormat_RGB_32_F, "Blurred"}}});

    std::vector<float> weights({0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f});

    m_horizontalBlur = ScreenSpaceEffect(drawContext, program, "HorizontalBlur");
    m_horizontalBlur.draw().sampler("Input").setTexture(inputSurface);
    m_horizontalBlur.draw().uniform("Weight").set(weights);
    m_horizontalBlur.draw().uniform("Horizontal").set(true);
    m_horizontalBlur.draw().setFramebuffer(m_intermediateBuffer);

    m_verticalBlur = ScreenSpaceEffect(drawContext, program, "VerticalBlur");
    m_verticalBlur.draw().sampler("Input").setTexture(m_intermediateBuffer.colorTargetRef("Blurred"));
    m_verticalBlur.draw().uniform("Weight").set(weights);
    m_verticalBlur.draw().uniform("Horizontal").set(false);
    m_verticalBlur.draw().setFramebuffer(m_outputBuffer);

    m_initialized = true;
}

void GaussianBlur::render()
{
    Assert(m_initialized, "");

    m_horizontalBlur.render();
    m_verticalBlur.render();
}

}