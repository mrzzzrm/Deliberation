#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureBinding.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
    struct BufferBinding;
}

class Buffer;
class BufferLayoutField;
class Context;
class DrawState;

class Draw final
{
public:
    Draw();

    bool isComplete() const;

    const std::string & name() const;

//    bool hasOutput() const;
//    const DrawOutputConfig & output() const;

    DrawState & state();
    const DrawState & state() const;

    Uniform uniform(const std::string & name);

    TextureBinding texture(const std::string & name);

//    void setOutput(const DrawOutputConfig & output);

    void setState(const DrawState & state);

    void setIndexBuffer(const Buffer & buffer);
    void addVertexBuffer(const Buffer & buffer);
    void addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count);
    void addInstanceBuffer(const Buffer & buffer, unsigned int divisor = 1u);
    void addInstanceBufferRange(const Buffer & buffer, unsigned int first, unsigned int count, unsigned int divisor = 1u);

    void schedule();

    std::string toString() const;

private:
    friend class Context;
    friend class detail::DrawExecution;
    friend class DrawVerification;

private:
    Draw(const std::shared_ptr<detail::DrawImpl> & impl);

    bool isBuild() const;

    void build();

    void verifyVertexBuffer(const Buffer & buffer) const;
    void verifyInstanceBuffer(const Buffer & buffer) const;

    const BufferLayoutField * bufferField(const std::string & name,
                                          detail::BufferBinding ** o_binding,
                                          gl::GLuint * o_divisor,
                                          unsigned int * o_count) const;

    std::string statusString() const;

private:
    std::shared_ptr<detail::DrawImpl> m_impl;
};

}

#include <Deliberation/Draw/Draw.inl>
