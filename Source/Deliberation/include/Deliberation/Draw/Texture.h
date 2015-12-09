#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class TextureImpl;
}

class Texture final
{
public:
    Texture();
    ~Texture();

    unsigned int width() const;
    unsigned int height() const;

    gl::GLenum type() const;

    unsigned int numFaces() const;

//    Surface & surface(unsigned int face);
//    const Surface & surface(unsigned int face) const;

private:
    Texture(const std::shared_ptr<detail::TextureImpl> & impl);

private:
    friend class Context;
    friend class TextureUploader;
    friend class detail::DrawExecution;

private:
    std::shared_ptr<detail::TextureImpl> m_impl;
};

}

