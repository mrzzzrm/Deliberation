#include <Deliberation/Font/LabelRenderer.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/Context.h>

#include <Deliberation/Font/Label.h>

namespace deliberation
{

LabelRenderer::LabelRenderer():
    m_context(nullptr)
{

}

LabelRenderer::LabelRenderer(Context & context):
    m_context(&context)
{
    m_program = m_context->createProgram({"../Data/Font/LabelRenderer.vert",
                                          "../Data/Font/LabelRenderer.frag"});

    std::vector<glm::vec2> vertices({
        {-0.5f, -0.5f},
        {-0.5f,  0.5f},
        { 0.5f, -0.5f},
        { 0.5f,  0.5f}
    });

    BufferLayout layout = m_context->createPackedBufferLayout<glm::vec2>({"Position"});
    m_vertexBuffer = m_context->createBuffer(layout);
    m_vertexBuffer.createUpload(vertices).schedule();

    m_draw = m_context->createDraw(m_program, gl::GL_TRIANGLE_STRIP, "LabelRenderer");
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.state().setDepthState({false, false});
    m_draw.state().setCullState(CullState::disabled());

    m_textureBinding = m_draw.texture("Texture");
    m_colorUniform = m_draw.uniform("Color");
    m_transformUniform = m_draw.uniform("Transform");
}

void LabelRenderer::render(const Label & label, const Viewport & viewport)
{
    Assert(m_context, "Hollow LabelRenderer can't render");

    m_textureBinding.set(label.texture());
    m_colorUniform.set(label.color());
    m_transformUniform.set(label.transform(viewport));

    m_draw.schedule();
}

}

