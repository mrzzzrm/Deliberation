#include <Deliberation/Scene/Effects/GaussianBlur.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

GaussianBlur::GaussianBlur(DrawContext & drawContext)
{
    auto program = drawContext.createProgram({
                                                 DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                                 DeliberationDataPath("Data/Shaders/GaussianBlur.frag")
                                             });

    m_maxNumSamples = program.interface().uniformBlockRef("Config").layout().field("Weights").arraySize();

    m_blur = ScreenSpaceEffect(drawContext, program, "GaussianBlur");
    m_inputSampler =  m_blur.draw().sampler("Input");
    m_inputSampler.setWrap(TextureWrap::ClampToEdge);
    m_inputSampler.setMagFilter(TextureFilter::Linear);
    m_inputSampler.setMinFilter(TextureFilter::Linear);
    m_horizontalUniform = m_blur.draw().uniform("Horizontal");

    m_configBlockLayout = m_blur.draw().program().interface().uniformBlockRef("Config").layout();

    m_configBuffer = m_blur.draw().uniformBuffer("Config");
}

void GaussianBlur::setInput(const Surface & surface)
{
    m_inputSampler.setTexture(surface);
}

void GaussianBlur::setFramebuffer(Framebuffer & framebuffer, const FramebufferMappings & mappings)
{
    m_blur.draw().setFramebuffer(framebuffer, mappings);
}

void GaussianBlur::setConfigBuffer(const Buffer & buffer)
{
    m_configBuffer.setBuffer(buffer);
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