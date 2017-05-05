#include <Deliberation/Font/LabelRenderer.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Font/Label.h>

namespace deliberation
{

LabelRenderer::LabelRenderer():
    m_drawContext(nullptr)
{

}

LabelRenderer::LabelRenderer(DrawContext & drawContext):
    m_drawContext(&drawContext)
{
    m_program = m_drawContext->createProgram({deliberation::DeliberationDataPath("Data/Font/LabelRenderer.vert"),
                                          deliberation::DeliberationDataPath("Data/Font/LabelRenderer.frag")});

    std::vector<glm::vec2> vertices({
        {-0.5f, -0.5f},
        {-0.5f,  0.5f},
        { 0.5f, -0.5f},
        { 0.5f,  0.5f}
    });

    DataLayout layout = DataLayout({"Position", Type_Vec2});
    m_vertexBuffer = m_drawContext->createBuffer(layout);
    m_vertexBuffer.upload(vertices);

    m_draw = m_drawContext->createDraw(m_program, DrawPrimitive::TriangleStrip, "LabelRenderer");
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setBlendState({BlendEquation::Add, BlendFactor::SourceAlpha, BlendFactor::One});

    m_sampler = m_draw.sampler("Texture");
    m_sampler.setWrap(TextureWrap::ClampToEdge);

    m_colorUniform = m_draw.uniform("Color");
    m_transformUniform = m_draw.uniform("Transform");
}

void LabelRenderer::render(const Label & label, const Viewport & viewport)
{
    Assert(m_drawContext, "Hollow LabelRenderer can't render");

    m_sampler.setTexture(label.texture());
    m_colorUniform.set(label.color());
    m_transformUniform.set(label.transform(viewport));

    m_draw.state().setViewport(viewport);

    m_draw.render();
}

}

