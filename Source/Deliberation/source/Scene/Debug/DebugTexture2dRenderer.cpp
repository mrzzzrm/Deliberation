#include <Deliberation/Scene/Debug/DebugTexture2dRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

DebugTexture2dRenderer::DebugTexture2dRenderer(DrawContext & drawContext, const Texture & texture)
{
    const auto program = context.createProgram({
       deliberation::DeliberationDataPath("Data/Shaders/DebugTexture2dRenderer.vert"),
       deliberation::DeliberationDataPath("Data/Shaders/DebugTexture2dRenderer.frag")
    });

    m_draw = context.createDraw(program, gl::GL_TRIANGLES);

    LayoutedBlob vertices({"Position", Type_Vec2}, 4);
    vertices.field<glm::vec2>("Position").assign({
        glm::vec2(-1.0f, -1.0f),
        glm::vec2( 1.0f, -1.0f),
        glm::vec2( 1.0f,  1.0f),
        glm::vec2(-1.0f,  1.0f)
    });

    LayoutedBlob indices({"Index", Type_U32}, 6);
    indices.field<u32>("Index").assign({0, 1, 3, 1, 2, 3});

    m_draw.addVertices(vertices);
    m_draw.setIndices(indices);
    m_draw.state().setDepthState({});
    m_draw.sampler("Texture").setTexture(texture);
}

void DebugTexture2dRenderer::render()
{
    m_draw.schedule();
}

}