#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/VertexAttribute.h>
#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Sampler.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
    struct BufferBinding;
}

class Buffer;
class DrawContext;
class Framebuffer;
class DrawState;

class Draw final
{
public:
    Draw();

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
    Buffer addInstances(const LayoutedBlob & data, u32 divisor = 1u);

    void setIndexBuffer(const Buffer & buffer);
    void setIndexBufferRange(const Buffer & buffer, u32 first, u32 count);
    void addVertexBuffer(const Buffer & buffer);
    void addVertexBufferRange(const Buffer & buffer, u32 first, u32 count);
    void addInstanceBuffer(const Buffer & buffer, u32 divisor = 1u);
    void addInstanceBufferRange(const Buffer & buffer, u32 first, u32 count, u32 divisor = 1u);

    template<typename T>
    void setAttribute(const std::string & name, const T & value);

    template<typename T>
    void setAttribute(size_t index, const T & value);

    template<typename T>
    VertexAttribute<T> attribute(const std::string & name);

    void setFramebuffer(const Framebuffer & framebuffer);

    void setRenderTarget(unsigned int index, Surface * surface);
    void setRenderTarget(const std::string & name, Surface * surface);

    void setUniformBuffer(const std::string & name, const Buffer & buffer, unsigned int begin = 0);

    void schedule() const;

    std::string toString() const;

private:
    friend class DrawContext;
    friend class detail::DrawExecution;
    friend class DrawVerification;
    friend void detail::SetVertexAttribute(Draw & draw, const ProgramInterfaceVertexAttribute & attribute, const void * data);

private:
    Draw(const std::shared_ptr<detail::DrawImpl> & impl);

    void build() const;

    void addVertexBuffer(const Buffer & buffer, bool ranged, u32 first, u32 count, u32 divisor);
    void setAttribute(const ProgramInterfaceVertexAttribute & attribute, const void * data);

private:
    std::shared_ptr<detail::DrawImpl> m_impl;
};

}

#include <Deliberation/Draw/Draw.inl>
