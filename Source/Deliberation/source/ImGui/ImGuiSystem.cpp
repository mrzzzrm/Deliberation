#include <Deliberation/ImGui/ImGuiSystem.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/TextureLoader.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/KeyMap.h>

namespace deliberation
{

ImGuiSystem::ImGuiSystem(World & world):
    Base(world),
    InputLayer(200),
    m_context(world.system<ApplicationSystem>().context()),
    m_input(world.system<ApplicationSystem>().input())
{
    auto & io = ImGui::GetIO();

    io.RenderDrawListsFn = nullptr;

    /**
     * Setup draw
     */
    auto vertexLayout = DataLayout();
    vertexLayout.addField({"Position", Type_Vec2, offsetof(ImDrawVert, pos)});
    vertexLayout.addField({"UV", Type_Vec2, offsetof(ImDrawVert, uv)});
    vertexLayout.addField({"Color", Type_U8Vec4, offsetof(ImDrawVert, col)});

    auto indexLayout = DataLayout("Index", sizeof(ImDrawIdx) == 2 ? Type_U16 : Type_U32);

    m_vertexBuffer = m_context.createBuffer(vertexLayout);
    m_indexBuffer = m_context.createBuffer(indexLayout);

    auto program = m_context.createProgram({
                                             DeliberationDataPath("Data/Shaders/ImGui.vert"),
                                             DeliberationDataPath("Data/Shaders/ImGui.frag")
                                         });

    m_draw = m_context.createDraw(program);

    m_draw.addVertexBuffer(m_vertexBuffer);

    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setDepthState(DepthState::disabledR());

    m_projectionUniform = m_draw.uniform("Projection");

    /**
     * Setup font atlas
     */
    unsigned char * pixels;
    int width;
    int height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    const auto textureBinary =
        TextureLoader(Blob::fromRawData(pixels, width * height * 4), width, height, PixelFormat_RGBA_8_U).load();
    const auto texture = m_context.createTexture(textureBinary);

    auto sampler = m_draw.sampler("Texture");
    sampler.setTexture(texture);
    io.Fonts->TexID = (void *)(intptr_t)&texture;
    
    /**
     * Setup input
     */
    io.KeyMap[ImGuiKey_Tab] = Key_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = Key_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = Key_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = Key_UP;
    io.KeyMap[ImGuiKey_DownArrow] = Key_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = Key_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = Key_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = Key_HOME;
    io.KeyMap[ImGuiKey_End] = Key_END;
    io.KeyMap[ImGuiKey_Delete] = Key_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = Key_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = Key_ENTER;
    io.KeyMap[ImGuiKey_Escape] = Key_ESCAPE;
    io.KeyMap[ImGuiKey_A] = Key_A;
    io.KeyMap[ImGuiKey_C] = Key_C;
    io.KeyMap[ImGuiKey_V] = Key_V;
    io.KeyMap[ImGuiKey_X] = Key_X;
    io.KeyMap[ImGuiKey_Y] = Key_Y;
    io.KeyMap[ImGuiKey_Z] = Key_Z;
}

void ImGuiSystem::onFrameBegin()
{
    auto & io = ImGui::GetIO();

    const auto & backbuffer = m_context.backbuffer();

    io.DisplaySize = ImVec2((float)backbuffer.width(), (float)backbuffer.height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    auto mouseX = (m_input.mousePosition().x + 1.0f) / 2.0f * backbuffer.width();
    auto mouseY = (1.0f - (m_input.mousePosition().y + 1.0f) / 2.0f) * backbuffer.height();

   // io.MouseDrawCursor = true;

    io.MousePos = ImVec2(mouseX, mouseY);

    io.MouseDown[0] = m_input.mouseButtonDown(MouseButton_Left);
    io.MouseDown[1] = m_input.mouseButtonDown(MouseButton_Right);
    io.MouseDown[2] = false;
    io.MouseDown[3] = false;
    io.MouseDown[4] = false;

    for (size_t k = 0; k < 512; k++) {
        io.KeysDown[k] = false;//m_input.keyDown(k);
    }
    
    io.KeyCtrl = io.KeysDown[Key_LEFT_CONTROL] || io.KeysDown[Key_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[Key_LEFT_SHIFT] || io.KeysDown[Key_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[Key_LEFT_ALT] || io.KeysDown[Key_RIGHT_ALT];
    io.KeySuper = io.KeysDown[Key_LEFT_SUPER] || io.KeysDown[Key_RIGHT_SUPER];

    io.DeltaTime = 1.0f / 60.0f;

    ImGui::NewFrame();

    m_wantsCaptureMouse = io.WantCaptureMouse;
}

void ImGuiSystem::onRender()
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

        m_vertexBuffer.scheduleRawUpload(Blob::fromRawData(vertexData, vertexDataSize), cmdList->VtxBuffer.Size);
        m_indexBuffer.scheduleRawUpload(Blob::fromRawData(indexData, indexDataSize), cmdList->IdxBuffer.Size);

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

            m_draw.schedule();

            indexOffset += drawCmd.ElemCount;
        }
    }
}

void ImGuiSystem::onMouseButtonPressed(MouseButtonEvent & event)
{
    if (m_wantsCaptureMouse) event.consume();
}

void ImGuiSystem::onMouseButtonDown(MouseButtonEvent & event)
{
    if (m_wantsCaptureMouse) event.consume();
}

}