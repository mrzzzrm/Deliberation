#pragma once

#include <memory>

#include <glbinding/gl/types.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class TextureImpl;
}

class Surface;

class Texture final
{
public:
    Texture();
    ~Texture();

    unsigned int width() const;
    unsigned int height() const;

    gl::GLenum type() const;

    unsigned int numFaces() const;

    Surface & surface(unsigned int face = 0);
    const Surface & surface(unsigned int face = 0) const;

private:
    friend class Context;
    friend class detail::DrawExecution;
    friend class TextureUploader;
    friend class Sampler;
    friend class Surface;

private:
    Texture(const std::shared_ptr<detail::TextureImpl> & impl);

private:
    std::shared_ptr<detail::TextureImpl> m_impl;
};

}

