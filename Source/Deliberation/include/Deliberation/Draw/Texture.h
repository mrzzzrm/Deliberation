#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
}

class Texture final
{
public:
    unsigned int width() const;
    unsigned int height() const;

    gl::GLenum type() const;

    unsigned int numFaces() const;

//    Surface & surface(unsigned int face);
//    const Surface & surface(unsigned int face) const;

private:
    Texture(gl::GLuint glName,
            unsigned int width,
            unsigned int height,
            unsigned int numFaces/*,
            const std::vector<Surface> & surfaces*/);

private:
    friend class Context;
    friend class TextureUploader;
    friend class detail::DrawExecution;

private:
    // Not using impl for these (yet), might happen at some point
    gl::GLuint m_glName;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_numFaces;
    gl::GLenum m_type;
//    std::vector<Surface> m_surfaces;
};

}

