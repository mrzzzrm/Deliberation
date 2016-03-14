#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Detail/NamedDataMemberOf.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawState.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Query.h>
#include <Deliberation/Draw/SurfaceDownload.h>
#include <Deliberation/Draw/TextureUpload.h>

namespace deliberation
{

namespace detail
{
    class BufferImpl;
    class QueryImpl;
}

class DELIBERATION_API Context final
{
public:
    Context(unsigned int backbufferWidth = 640u, unsigned int backbufferHeight = 480u);

    Framebuffer backbuffer();
    const Framebuffer & backbuffer() const;

    void setBackbufferResolution(unsigned int width, unsigned height);

    template<typename T>
    BufferLayout createBufferLayout(const std::vector<detail::NamedDataMemberOf<T>> & members);

    template<typename ... Types>
    BufferLayout createPackedBufferLayout(const std::vector<std::string> & names);

    Buffer createBuffer(const BufferLayout & layout);
    Buffer createIndexBuffer8();
    Buffer createIndexBuffer16();
    Buffer createIndexBuffer32();

    Program createProgram(const std::vector<std::string> & paths);

    Draw createDraw(Program & program, gl::GLenum primitive = gl::GL_TRIANGLES, const std::string & name = std::string());
    Draw createDraw(Program & program, const DrawState & drawState, const std::string & name = std::string());

    Clear createClear();
    Clear createClear(Framebuffer & framebuffer);

    Texture createTexture(const TextureBinary & binary);
    Texture createTexture2D(unsigned int width,
                            unsigned int height,
                            PixelFormat format,
                            bool clear = true);

    Framebuffer createFramebuffer(unsigned int width, unsigned int height);

    Query createQuery(QueryType type);

    /*
        TODO
            move these to private: they shouldn't be called by anyone else than Deliberation/Draw
    */
    void allocateBuffer(detail::BufferImpl & buffer);
    void deallocateBuffer(detail::BufferImpl & buffer);
    void scheduleBufferUpload(const BufferUpload & upload);
    void scheduleTextureUpload(const TextureUpload & upload);
    void scheduleDraw(const Draw & draw);
    void scheduleClear(const Clear & clear);

    Program & blitProgram();
    Buffer & blitVertexBuffer();
    /*
        / move?
    */

private:
    friend class detail::QueryImpl;
    friend class SurfaceDownloadImpl;

private:
    void executeBufferUpload(const BufferUpload & upload);
    void executeTextureUpload(const TextureUpload & upload);
    void executeDraw(const Draw & draw);
    void executeClear(const Clear & draw);

private:
    Framebuffer                 m_backbuffer;
    GLStateManager              m_glStateManager;
    std::vector<BufferUpload>   m_bufferUploads;

    Optional<Program>           m_blitProgram;
    Optional<Buffer>            m_blitVertexBuffer;
};

};

#include <Deliberation/Draw/Context.inl>
