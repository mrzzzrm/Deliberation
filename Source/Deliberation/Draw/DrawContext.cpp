#include <Deliberation/Draw/DrawContext.h>

#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Log.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include "Detail/BufferImpl.h"
#include "Detail/ClearImpl.h"
#include "Detail/DrawImpl.h"
#include "Detail/FramebufferImpl.h"
#include "Detail/ProgramImpl.h"
#include "Detail/QueryImpl.h"
#include "Detail/TextureImpl.h"

#define VERBOSE 0

namespace deliberation
{
DELIBERATION_DEFINE_GLOBAL(DrawContext);

DrawContext::DrawContext(
    unsigned int backbufferWidth, unsigned int backbufferHeight)
{
    m_backbuffer = Framebuffer(
        FramebufferImpl::backbuffer(*this));
}

Framebuffer & DrawContext::backbuffer() { return m_backbuffer; }

const Framebuffer & DrawContext::backbuffer() const { return m_backbuffer; }

void DrawContext::setBackbufferResolution(unsigned int width, unsigned height)
{
    if (m_backbuffer.width() == width && m_backbuffer.height() == height)
    {
        return;
    }

    m_backbuffer =
        Framebuffer(FramebufferImpl::backbuffer(*this, width, height));
}

Buffer DrawContext::createBuffer(const DataLayout & layout, size_t count)
{
    auto buffer = Buffer(std::make_shared<BufferImpl>(*this, layout));
    if (count != 0) buffer.reinit(count);
    return buffer;
}

Buffer DrawContext::createBuffer(const LayoutedBlob & data)
{
    auto buffer = createBuffer(data.layout());
    buffer.upload(data);
    return buffer;
}

Program DrawContext::createProgram(const std::vector<std::string> & paths)
{
    Program program(std::make_shared<ProgramImpl>(*this, paths));
    return program;
}

Draw DrawContext::createDraw(
    const Program & program, DrawPrimitive primitive, const std::string & name)
{
    auto impl = std::make_shared<DrawImpl>(*this, program);
    impl->state.rasterizerState().setPrimitive(primitive);
    impl->name = name;

    return Draw(impl);
}

Draw DrawContext::createDraw(
    const Program &     program,
    const DrawState &   drawState,
    const std::string & name)
{
    auto impl = std::make_shared<DrawImpl>(*this, program);
    impl->state = drawState;
    impl->name = name;

    return Draw(impl);
}

Clear DrawContext::createClear()
{
    return Clear(std::make_shared<ClearImpl>(*this, backbuffer().m_impl));
}

Clear DrawContext::createClear(Framebuffer & framebuffer)
{
    return Clear(std::make_shared<ClearImpl>(*this, framebuffer.m_impl));
}

Texture DrawContext::createTexture(const TextureBinary & binary)
{
    auto impl = std::make_shared<TextureImpl>(
        *this,
        binary.width(),
        binary.height(),
        binary.numFaces(),
        binary.format());

    impl->setupSurfaces(impl);

    auto texture = Texture(impl);
    texture.upload(binary);

    return texture;
}

Texture DrawContext::createTexture2D(u32 width, u32 height, PixelFormat format)
{
    AssertM(width > 0 && height > 0, "Invalid size");

    auto impl = std::make_shared<TextureImpl>(*this, width, height, 1, format);
    impl->setupSurfaces(impl);

    auto texture = Texture(impl);

    return texture;
}

Framebuffer
DrawContext::createFramebuffer(const FramebufferDesc & framebufferDesc)
{
    DELIBERATION_LOG_INNER_SCOPE("DrawContext")

    AssertM(
        framebufferDesc.width > 0 && framebufferDesc.height > 0,
        "Framebuffer axes must be > 0");

    auto framebuffer =
        Framebuffer(FramebufferImpl::custom(*this, framebufferDesc));

#if VERBOSE
    Log->info("Creating Framebuffer: {}", framebuffer.toString());
#endif

    return framebuffer;
}

Query DrawContext::createQuery(QueryType type)
{
    return Query(std::make_shared<QueryImpl>(*this, type));
}
};
