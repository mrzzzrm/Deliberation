#include <Deliberation/Draw/Context.h>

#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/TextureLoader.h>

#include "Detail/BufferImpl.h"
#include "Detail/ClearImpl.h"
#include "Detail/DrawExecution.h"
#include "Detail/DrawImpl.h"
#include "Detail/FramebufferImpl.h"
#include "Detail/ProgramImpl.h"
#include "Detail/SurfaceDownloadImpl.h"
#include "Detail/TextureImpl.h"
#include "BufferUploadExecution.h"
#include "ClearExecution.h"
#include "DrawVerification.h"
#include "TextureUploadExecution.h"

namespace deliberation
{

Context::Context(unsigned int backbufferWidth, unsigned int backbufferHeight)
{
    std::cout << "Creating Deliberation Context" << std::endl;

    m_backbuffer = Framebuffer(detail::FramebufferImpl::backbuffer(backbufferWidth, backbufferHeight));
}

Framebuffer Context::backbuffer()
{
    return Framebuffer(detail::FramebufferImpl::backbuffer(m_backbuffer.width(), m_backbuffer.height()));
}

const Framebuffer & Context::backbuffer() const
{
    return m_backbuffer;
}

void Context::setBackbufferResolution(unsigned int width, unsigned height)
{
    if (m_backbuffer.width() == width && m_backbuffer.height() == height)
    {
        return;
    }

    m_backbuffer = Framebuffer(detail::FramebufferImpl::backbuffer(width, height));
}

Buffer Context::createBuffer(const BufferLayout & layout)
{
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer Context::createIndexBuffer8()
{
    BufferLayout layout({BufferLayoutField("Index8", sizeof(gl::GLubyte), gl::GL_UNSIGNED_BYTE, 0)});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer Context::createIndexBuffer16()
{
    BufferLayout layout({BufferLayoutField("Index16", sizeof(gl::GLushort), gl::GL_UNSIGNED_SHORT, 0)});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Buffer Context::createIndexBuffer32()
{
    BufferLayout layout({BufferLayoutField("Index32", sizeof(gl::GLuint), gl::GL_UNSIGNED_INT, 0)});
    return Buffer(std::make_shared<detail::BufferImpl>(*this, layout));
}

Program Context::createProgram(const std::vector<std::string> & paths)
{
    Program program(std::make_shared<detail::ProgramImpl>(paths));
    return program;
}

Draw Context::createDraw(Program & program, gl::GLenum primitive, const std::string & name)
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state.setPrimitive(primitive);
    impl->name = name;

    return Draw(impl);
}

Draw Context::createDraw(Program & program, const DrawState & drawState, const std::string & name)
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state = drawState;
    impl->name = name;

    return Draw(impl);
}

Clear Context::createClear()
{
    return Clear(std::make_shared<detail::ClearImpl>(*this));
}

Clear Context::createClear(const glm::vec4 & color)
{
    auto clear = createClear();
    clear.setColor(color);

    return clear;
}

Texture Context::createTexture(const TextureBinary & binary)
{
    auto texture = Texture(detail::TextureImpl::build(*this, binary.width(), binary.height(), 1, binary.format()));
    texture.createUpload(binary).schedule();

    return texture;
}

Texture Context::createTexture2D(unsigned int width,
                                 unsigned int height,
                                 PixelFormat format,
                                 bool clear)
{
    auto texture = Texture(detail::TextureImpl::build(*this, width, height, 1, format));

    if (clear)
    {
        auto c = createClear();
        if (PixelFormatType(format) == PixelFormatType_Color) {
            c.framebuffer().setRenderTarget(0, &texture.surface());
        }
        else {
            c.framebuffer().setDepthTarget(&texture.surface());
        }
        c.schedule();
    }

    return texture;
}

SurfaceDownload Context::createSurfaceDownload(const Surface & surface)
{
    return SurfaceDownload(std::make_shared<SurfaceDownloadImpl>(surface));
}

void Context::allocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName == 0u, "");

    gl::GLuint glName;
    gl::glGenBuffers(1, &glName);
    Assert(glName != 0u, "");

    buffer.glName = glName;
}

void Context::deallocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName != 0u, "");

    m_glStateManager.deleteBuffer(buffer.glName);

    buffer.glName = 0u;
}

void Context::scheduleBufferUpload(const BufferUpload & upload)
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

void Context::scheduleTextureUpload(const TextureUpload & upload)
{
    executeTextureUpload(upload);
}

void Context::scheduleDraw(const Draw & draw)
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

void Context::scheduleClear(const Clear & clear)
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

void Context::executeBufferUpload(const BufferUpload & upload)
{
    BufferUploadExecution(m_glStateManager, upload).perform();
}

void Context::executeTextureUpload(const TextureUpload & upload)
{
    TextureUploadExecution(m_glStateManager, upload).perform();
}

void Context::executeDraw(const Draw & draw)
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

void Context::executeClear(const Clear & clear)
{
    ClearExecution(m_glStateManager, clear).perform();
}

};

