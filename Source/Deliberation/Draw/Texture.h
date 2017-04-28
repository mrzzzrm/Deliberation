#pragma once

#include <memory>

#include <glbinding/gl/types.h>



#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/TextureUpload.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class TextureImpl;
}

class Surface;
class TextureBinary;

class Texture final
{
public:
    Texture();

    unsigned int width() const;
    unsigned int height() const;

    gl::GLenum type() const;

    PixelFormat format() const;

    unsigned int numFaces() const;

    Surface & surface(unsigned int face = 0);
    const Surface & surface(unsigned int face = 0) const;

    TextureUpload createUpload(const TextureBinary & binary);
    void scheduleUpload(const TextureBinary & binary);

private:
    friend class DrawContext;
    friend class detail::DrawExecution;
    friend class TextureUploadExecution;
    friend class Sampler;
    friend class Surface;

private:
    Texture(const std::shared_ptr<detail::TextureImpl> & impl);

private:
    std::shared_ptr<detail::TextureImpl> m_impl;
};

}

