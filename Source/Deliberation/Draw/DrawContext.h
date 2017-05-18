#pragma once

#include <vector>

#include "SDL.h"

#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawState.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Query.h>
#include <Deliberation/Draw/RasterizerState.h>
#include <Deliberation/Draw/SurfaceDownload.h>

namespace deliberation
{
namespace detail
{
    class BufferImpl;
    class QueryImpl;
}

class DrawContext final
{
  public:
    DrawContext(
        unsigned int backbufferWidth = 640u,
        unsigned int backbufferHeight = 480u);

    Framebuffer &       backbuffer();
    const Framebuffer & backbuffer() const;

    void setBackbufferResolution(unsigned int width, unsigned height);

    Buffer createBuffer(const DataLayout & layout, size_t count = 0);
    Buffer createBuffer(const LayoutedBlob & data);

    Program createProgram(const std::vector<std::string> & paths);

    Draw createDraw(
        const Program &     program,
        DrawPrimitive       primitive = DrawPrimitive::Triangles,
        const std::string & name = std::string());
    Draw createDraw(
        const Program &     program,
        const DrawState &   drawState,
        const std::string & name = std::string());

    Clear createClear();
    Clear createClear(Framebuffer & framebuffer);

    Texture createTexture(const TextureBinary & binary);
    Texture createTexture2D(
        unsigned int width, unsigned int height, PixelFormat format);

    Framebuffer createFramebuffer(const FramebufferDesc & framebufferDesc);

    Query createQuery(QueryType type);

  private:
    friend class Buffer;
    friend class BufferImpl;
    friend class Clear;
    friend class Draw;
    friend class DrawImpl;
    friend class FramebufferImpl;
    friend class QueryImpl;
    friend class SurfaceDownloadImpl;
    friend class Texture;
    friend class TextureImpl;

  private:
    Framebuffer    m_backbuffer;
    GLStateManager m_glStateManager;
};
};
