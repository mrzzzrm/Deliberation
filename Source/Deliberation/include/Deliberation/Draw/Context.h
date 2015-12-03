#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Detail/NamedDataMemberOf.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawState.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{

namespace detail
{
    class BufferImpl;
}

class DELIBERATION_API Context final
{
public:
    Context();

    template<typename T>
    BufferLayout createBufferLayout(const std::vector<detail::NamedDataMemberOf<T>> & members);

    template<typename ... Types>
    BufferLayout createPackedBufferLayout(const std::vector<std::string> & names);

    Buffer createBuffer(const BufferLayout & layout);
    Buffer createIndexBuffer32();

    template<typename T>
    BufferUpload createBufferUpload(Buffer & buffer, const std::vector<T> & data);

    template<typename T>
    BufferUpload createBufferUpload(Buffer & buffer, const std::vector<T> && data);

    template<typename T>
    BufferUpload createRawBufferUpload(Buffer & buffer, const std::vector<T> & data, unsigned int count);

    template<typename T>
    BufferUpload createRawBufferUpload(Buffer & buffer, const std::vector<T> && data, unsigned int count);

    Program createProgram(const std::vector<std::string> & paths);

    Draw createDraw(Program & program, gl::GLenum primitive, const std::string & name = std::string());
    Draw createDraw(Program & program, const DrawState & drawState, const std::string & name = std::string());

    Clear createClear();

    Texture createTexture(const std::string & path);

    /*
        TODO
            move these to private: they shouldn't be called by anyone else than Deliberation/Draw
    */
    void allocateBuffer(detail::BufferImpl & buffer);
    void deallocateBuffer(detail::BufferImpl & buffer);
    void scheduleBufferUpload(const BufferUpload & upload);
    void scheduleDraw(const Draw & draw);
    void scheduleClear(const Clear & clear);

private:
    void executeBufferUpload(const BufferUpload & upload);
    void executeDraw(const Draw & draw);
    void executeClear(const Clear & draw);

private:
    GLStateManager m_glStateManager;
    std::vector<BufferUpload> m_bufferUploads;
};

};

#include <Deliberation/Draw/Context.inl>
