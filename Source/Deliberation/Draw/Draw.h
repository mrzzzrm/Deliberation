#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/BufferTextureBinding.h>
#include <Deliberation/Draw/FramebufferBinding.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Sampler.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Uniform.h>
#include <Deliberation/Draw/UniformBufferHandle.h>
#include <Deliberation/Draw/VertexAttribute.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class Buffer;
class DrawContext;
class Framebuffer;
class DrawState;

class Draw final:
    public AbstractDrawObject<DrawImpl>
{
public:
    Draw() = default;

    const std::string & name() const;

    Program program() const;

    DrawState &       state();
    const DrawState & state() const;

    Framebuffer &       framebuffer();
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
    void addInstanceBufferRange(
        const Buffer & buffer, u32 first, u32 count, u32 divisor = 1u);

    template<typename T>
    void setAttribute(const std::string & name, const T & value);

    VertexAttribute attribute(const std::string & name);

    /**
     * Shortcut for setFramebufferBinding(framebufferBinding(framebuffer, mapping))
     * @param framebuffer
     * @param mappings
     */
    void setFramebuffer(
        Framebuffer & framebuffer, const FramebufferMappings & mappings = {});

    /**
     * Create a FramebufferBinding for this Draw, but DO NOT use it (yet).
     * Draw::setFramebufferBinding(const FramebufferBinding&) has to be invoked separately
     * @param framebuffer
     * @param mappings
     * @return
     */
    FramebufferBinding framebufferBinding(
        Framebuffer &               framebuffer,
        const FramebufferMappings & mappings = {});

    void setFramebufferBinding(const FramebufferBinding & binding);

    void setUniformBuffer(
        const std::string & name,
        const Buffer &      buffer,
        unsigned int        begin = 0);

    UniformBufferHandle uniformBuffer(const std::string & name);

    void setBufferTexture(const std::string & name, const Buffer & buffer);

    void setBufferTextureRange(
        const std::string & name, const Buffer & buffer, u32 begin, u32 count);

    BufferTextureBinding bufferTexture(const std::string & name);

    void render() const;

    std::string toString() const;

private:
    friend class DrawContext;
    friend class DrawVerification;

private:
    Draw(const std::shared_ptr<DrawImpl> & impl);

    void build() const;

    void setAttribute(
        const ProgramInterfaceVertexAttribute & attribute, const void * data);
    void addVertexBuffer(
        const Buffer & buffer, bool ranged, u32 first, u32 count, u32 divisor);
};
}

#include <Deliberation/Draw/Draw.inl>
