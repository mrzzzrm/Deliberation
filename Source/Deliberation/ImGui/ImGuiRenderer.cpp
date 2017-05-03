#include <Deliberation/ImGui/ImGuiRenderer.h>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/TextureLoader.h>

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{

ImGuiRenderer::ImGuiRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::Overlay)
{
    // Setup Font Atlas has to happen before first ImGui::NewFrame();, thus: here
    auto & io = ImGui::GetIO();
    io.Fonts->GetTexDataAsRGBA32(&m_fontAtlasPixels, &m_fontAtlasWidth, &m_fontAtlasHeight);
}

void ImGuiRenderer::onSetupRender() 
{
    auto & io = ImGui::GetIO();
    
    /**
      * Setup draw
      */
    auto vertexLayout = DataLayout();
    vertexLayout.addField({"Position", Type_Vec2, offsetof(ImDrawVert, pos)});
    vertexLayout.addField({"UV", Type_Vec2, offsetof(ImDrawVert, uv)});
    vertexLayout.addField({"Color", Type_U8Vec4, offsetof(ImDrawVert, col)});

    auto indexLayout = DataLayout("Index", sizeof(ImDrawIdx) == 2 ? Type_U16 : Type_U32);

    m_vertexBuffer = drawContext().createBuffer(vertexLayout);
    m_indexBuffer = drawContext().createBuffer(indexLayout);

    auto program = drawContext().createProgram({
                                                   DeliberationDataPath("Data/Shaders/ImGui.vert"),
                                                   DeliberationDataPath("Data/Shaders/ImGui.frag")
                                               });

    m_draw = drawContext().createDraw(program);

    m_draw.addVertexBuffer(m_vertexBuffer);

    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setDepthState(DepthState::disabledR());

    m_projectionUniform = m_draw.uniform("Projection");

    /**
     * Setup font atlas
     */

    const auto textureBinary =
        TextureLoader(Blob::fromRawData(m_fontAtlasPixels,
                                        m_fontAtlasWidth * m_fontAtlasHeight * 4),
                      m_fontAtlasWidth,
                      m_fontAtlasHeight,
                      PixelFormat_RGBA_8_U).
            load();
    const auto texture = drawContext().createTexture(textureBinary);

    auto sampler = m_draw.sampler("Texture");
    sampler.setTexture(texture);
    io.Fonts->TexID = (void *)(intptr_t)&texture;
}

void ImGuiRenderer::render()
{
    auto & io = ImGui::GetIO();

    ImGui::Render();

    const auto * drawData = ImGui::GetDrawData();
    Assert(drawData->Valid, "");

    auto projection = glm::mat4(
        2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f,
        0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f,
        0.0f,                  0.0f,                  -1.0f, 0.0f,
        -1.0f,                  1.0f,                   0.0f, 1.0f
    );

    m_projectionUniform.set(projection);

    //   std::cout << "Lists: " << drawData->CmdListsCount << std::endl;
    for (int l = 0; l < drawData->CmdListsCount; l++)
    {
        const auto * cmdList = drawData->CmdLists[l];

        const auto * vertexData = cmdList->VtxBuffer.Data;
        const auto vertexDataSize = cmdList->VtxBuffer.Size * sizeof(ImDrawVert);

        const auto * indexData = cmdList->IdxBuffer.Data;
        const auto indexDataSize = cmdList->IdxBuffer.Size * sizeof(ImDrawIdx);

        u32 indexOffset = 0;

        m_vertexBuffer.rawUpload(Blob::fromRawData(vertexData, vertexDataSize), cmdList->VtxBuffer.Size);
        m_indexBuffer.rawUpload(Blob::fromRawData(indexData, indexDataSize), cmdList->IdxBuffer.Size);

        //   std::cout << "  " << l << ": Cmds: " << cmdList->CmdBuffer.Size << std::endl;
        for (int c = 0; c < cmdList->CmdBuffer.Size; c++)
        {
            const auto & drawCmd = cmdList->CmdBuffer[c];

//            std::cout << drawCmd.ElemCount << ": Ignoring clip rect: " << drawCmd.ClipRect.x << " " << drawCmd.ClipRect.y << " " <<
//                 drawCmd.ClipRect.z << " " << drawCmd.ClipRect.w << " Texture: " << drawCmd.TextureId << std::endl;

            const auto & clipRect = drawCmd.ClipRect;

            m_draw.state().rasterizerState().enableScissorRect((u32)clipRect.x,
                                                               (u32)(io.DisplaySize.y - clipRect.w),
                                                               (u32)(clipRect.z - clipRect.x),
                                                               (u32)(clipRect.w - clipRect.y));

            m_draw.setIndexBufferRange(m_indexBuffer, indexOffset, drawCmd.ElemCount);

            m_draw.render();

            indexOffset += drawCmd.ElemCount;
        }
    }

}

}
