#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Detail/NamedDataMemberOf.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawState.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/SurfaceDownload.h>
#include <Deliberation/Draw/TextureUpload.h>

namespace deliberation
{

namespace detail
{
    class BufferImpl;
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

    Draw createDraw(Program & program, gl::GLenum primitive, const std::string & name = std::string());
    Draw createDraw(Program & program, const DrawState & drawState, const std::string & name = std::string());

    Clear createClear();
    Clear createClear(const glm::vec4 & color);

    Texture createTexture(const TextureBinary & binary);
    Texture createTexture2D(unsigned int width,
                            unsigned int height,
                            PixelFormat format,
                            bool clear = true);

    SurfaceDownload createSurfaceDownload(const Surface & surface);

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

private:
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
};

};

#include <Deliberation/Draw/Context.inl>
