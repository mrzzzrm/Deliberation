#include <Deliberation/Scene/Effects/GaussianBlur.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

GaussianBlur::GaussianBlur(DrawContext & drawContext)
{
    auto program = drawContext.createProgram({
                                                 DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                                 DeliberationDataPath("Data/Shaders/GaussianBlur.frag")
                                             });

    std::vector<float> weights({0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f});

    m_blur = ScreenSpaceEffect(drawContext, program, "HorizontalBlur");
    m_inputSampler =  m_blur.draw().sampler("Input");
    m_inputSampler.setWrap(TextureWrap::ClampToEdge);
    m_inputSampler.setMagFilter(TextureFilter::Linear);
    m_inputSampler.setMinFilter(TextureFilter::Linear);

    m_blur.draw().uniform("Weight").set(weights);
    m_horizontalUniform = m_blur.draw().uniform("Horizontal");
}

void GaussianBlur::setInput(const Surface & surface)
{
    m_inputSampler.setTexture(surface);
}

void GaussianBlur::setFramebuffer(Framebuffer & framebuffer, const FramebufferBinding & fbBinding)
{
    m_blur.draw().setFramebuffer(framebuffer, fbBinding);
}

void GaussianBlur::renderHBlur()
{
    m_horizontalUniform.set(true);
    m_blur.render();
}

void GaussianBlur::renderVBlur()
{
    m_horizontalUniform.set(false);
    m_blur.render();
}

}