#include <Deliberation/Draw/DrawContext.h>

#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/TextureLoader.h>

#include "Detail/BufferImpl.h"
#include "Detail/ClearImpl.h"
#include "Detail/DrawExecution.h"
#include "Detail/DrawImpl.h"
#include "Detail/FramebufferImpl.h"
#include "Detail/ProgramImpl.h"
#include "Detail/QueryImpl.h"
#include "Detail/TextureImpl.h"
#include "BufferUploadExecution.h"
#include "ClearExecution.h"
#include "DrawVerification.h"
#include "TextureUploadExecution.h"

namespace deliberation
{

DrawContext::DrawContext(unsigned int backbufferWidth, unsigned int backbufferHeight)
{
    m_backbuffer = Framebuffer(detail::FramebufferImpl::backbuffer(*this, backbufferWidth, backbufferHeight));
}

Framebuffer DrawContext::backbuffer()
{
    return Framebuffer(detail::FramebufferImpl::backbuffer(*this, m_backbuffer.width(), m_backbuffer.height()));
}

const Framebuffer & DrawContext::backbuffer() const
{
    return m_backbuffer;
}

void DrawContext::setBackbufferResolution(unsigned int width, unsigned height)
{
    if (m_backbuffer.width() == width && m_backbuffer.height() == height)
    {
        return;
    }

    m_backbuffer = Framebuffer(detail::FramebufferImpl::backbuffer(*this, width, height));
}

Buffer DrawContext::createBuffer(const DataLayout & layout)
{
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer DrawContext::createBuffer(const LayoutedBlob & data)
{
    auto buffer = createBuffer(data.layout());
    buffer.scheduleUpload(data);
    return buffer;
}

Buffer DrawContext::createIndexBuffer8()
{
    DataLayout layout({{"Index8", Type_U8}});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer DrawContext::createIndexBuffer16()
{
    DataLayout layout({{"Index16", Type_U16}});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer DrawContext::createIndexBuffer32()
{
    DataLayout layout({{"Index32", Type_U32}});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Program DrawContext::createProgram(const std::vector<std::string> & paths)
{
    Program program(std::make_shared<detail::ProgramImpl>(*this, paths));
    return program;
}

Draw DrawContext::createDraw(const Program & program, gl::GLenum primitive, const std::string & name)
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state.rasterizerState().setPrimitive(primitive);
    impl->name = name;

    return Draw(impl);
}

Draw DrawContext::createDraw(const Program & program, const DrawState & drawState, const std::string & name)
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state = drawState;
    impl->name = name;

    return Draw(impl);
}

Clear DrawContext::createClear()
{
    return Clear(std::make_shared<detail::ClearImpl>(*this, backbuffer().m_impl));
}

Clear DrawContext::createClear(Framebuffer & framebuffer)
{
    return Clear(std::make_shared<detail::ClearImpl>(*this, framebuffer.m_impl));
}

Texture DrawContext::createTexture(const TextureBinary & binary)
{
    auto texture = Texture(detail::TextureImpl::build(*this, binary.width(),
                                                      binary.height(),
                                                      binary.numFaces(),
                                                      binary.format()));
    texture.scheduleUpload(binary);

    return texture;
}

Texture DrawContext::createTexture2D(unsigned int width,
                                 unsigned int height,
                                 PixelFormat format,
                                 bool clear)
{
    Assert(width > 0 && height > 0, "Invalid size");

    auto texture = Texture(detail::TextureImpl::build(*this, width, height, 1, format));

    if (clear)
    {
        auto c = createClear();
        if (format.pixelType() == PixelType_Color) {
            c.framebuffer().setRenderTarget(0, &texture.surface());
        }
        else {
            c.framebuffer().setDepthTarget(&texture.surface());
        }
        c.render();
    }

    return texture;
}

Framebuffer DrawContext::createFramebuffer(unsigned int width, unsigned int height)
{
    return Framebuffer(detail::FramebufferImpl::custom(*this, width, height));
}

Query DrawContext::createQuery(QueryType type)
{
    return Query(std::make_shared<detail::QueryImpl>(*this, type));
}

void DrawContext::allocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName == 0u, "");

    gl::GLuint glName;
    gl::glGenBuffers(1, &glName);
    Assert(glName != 0u, "");

    buffer.glName = glName;
}

void DrawContext::deallocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName != 0u, "");

    m_glStateManager.deleteBuffer(buffer.glName);

    buffer.glName = 0u;
}

void DrawContext::scheduleBufferUpload(const BufferUpload & upload)
{
//    switch(m_mode)
//    {
//    case Mode::Immediate:
        executeBufferUpload(upload);
//        break;
//    case Mode::Deferred:
//        m_bufferUploadCommands.push_back(command);
//        m_commands.push_back(CommandType::BufferUpload);
//        break;
//    default:
//        Fail("");
//   }
}

void DrawContext::scheduleTextureUpload(const TextureUpload & upload)
{
    executeTextureUpload(upload);
}

void DrawContext::scheduleDraw(const Draw & draw)
{
//    switch(m_mode)
//    {
//    case Mode::Immediate:
        executeDraw(draw);
//        break;
//    case Mode::Deferred:
//        m_drawCommands.push_back(command);
//        m_commands.push_back(CommandType::Draw);
//        break;
//    default:
//        Fail("");
//    }
}

void DrawContext::scheduleClear(const Clear & clear)
{
//    switch(m_mode)
//    {
//    case Mode::Immediate:
        executeClear(clear);
//        break;
//    case Mode::Deferred:
//          Fail("");
//        break;
//    default:
//        Fail("");
//    }
}

Program & DrawContext::blitProgram()
{
    if (!m_blitProgram.engaged())
    {
        m_blitProgram.reset(createProgram({
            deliberation::DeliberationDataPath("Data/Shaders/Blit.vert"),
            deliberation::DeliberationDataPath("Data/Shaders/Blit.frag")
        }));
    }
    return m_blitProgram.get();
}

Buffer & DrawContext::blitVertexBuffer()
{
    if (!m_blitVertexBuffer.engaged())
    {
        auto layout = DataLayout({{"Position", Type_Vec2}});
        m_blitVertexBuffer.reset(createBuffer(layout));

        std::vector<glm::vec2> vertices({
            {-1.0f, -1.0f},
            { 1.0f, -1.0f},
            {-1.0f,  1.0f},
            { 1.0f,  1.0f}
        });

        m_blitVertexBuffer.get().scheduleUpload(vertices);
    }
    return m_blitVertexBuffer.get();
}

void DrawContext::executeBufferUpload(const BufferUpload & upload)
{
    BufferUploadExecution(m_glStateManager, upload).perform();
}

void DrawContext::executeTextureUpload(const TextureUpload & upload)
{
    TextureUploadExecution(m_glStateManager, upload).perform();
}

void DrawContext::executeDraw(const Draw & draw)
{
//    if (!command.hasOutput())
//    {
//        command.setOutput(m_defaultOutput);
//    }

//    if (!command.state().hasViewport())
//    {
//        if (command.output().mode() == DrawOutputConfig::ToMultipleRenderTarget)
//        {
//           command.state().setViewport({0, 0, command.output().mrt().width(), command.output().mrt().height()});
//        }
//        else
//        {
//           command.state().setViewport({0, 0, 640, 480});
//        }
//    }

    DrawVerification verification(draw);
    Assert(verification.passed(), verification.toString());

   detail::DrawExecution(m_glStateManager, draw).perform();
}

void DrawContext::executeClear(const Clear & clear)
{
    ClearExecution(m_glStateManager, clear).perform();
}

};

