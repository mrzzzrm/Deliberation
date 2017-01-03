#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Sampler.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
    struct BufferBinding;
}

class Buffer;
class Context;
class Framebuffer;
class DrawState;

class DELIBERATION_API Draw final
{
public:
    Draw();

    bool isComplete() const;

    const std::string & name() const;

    Program program() const;

    DrawState & state();
    const DrawState & state() const;

    Framebuffer & framebuffer();
    const Framebuffer & framebuffer() const;

    Uniform uniform(const std::string & name);
    Sampler sampler(const std::string & name);

    void setState(const DrawState & state);

    Buffer setIndices(const LayoutedBlob & data);
    Buffer addVertices(const LayoutedBlob & data);
    Buffer addInstances(const LayoutedBlob & data, unsigned int divisor = 1u);

    void setIndexBuffer(const Buffer & buffer);
    void addVertexBuffer(const Buffer & buffer);
    void addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count);
    void addInstanceBuffer(const Buffer & buffer, unsigned int divisor = 1u);
    void addInstanceBufferRange(const Buffer & buffer, unsigned int first, unsigned int count, unsigned int divisor = 1u);

    void setFramebuffer(const Framebuffer & framebuffer);

    void setRenderTarget(unsigned int index, Surface * surface);
    void setRenderTarget(const std::string & name, Surface * surface);

    void setUniformBuffer(const std::string & name, const Buffer & buffer, unsigned int begin = 0);

    void schedule() const;

    std::string toString() const;

private:
    friend class Context;
    friend class detail::DrawExecution;
    friend class DrawVerification;

private:
    Draw(const std::shared_ptr<detail::DrawImpl> & impl);

    bool isBuild() const;

    void build() const;

    void verifyVertexBuffer(const Buffer & buffer) const;
    void verifyInstanceBuffer(const Buffer & buffer) const;

    const DataLayoutField * bufferField(const std::string & name,
                                          detail::BufferBinding ** o_binding,
                                          gl::GLuint * o_divisor,
                                          unsigned int * o_count) const;

    std::string statusString() const;

private:
    std::shared_ptr<detail::DrawImpl> m_impl;
};

}

#include <Deliberation/Draw/Draw.inl>
