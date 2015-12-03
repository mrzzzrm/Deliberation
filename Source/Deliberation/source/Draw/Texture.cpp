#include <Deliberation/Draw/Texture.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

unsigned int Texture::width() const
{
    return m_width;
}

unsigned int Texture::height() const
{
    return m_height;
}

unsigned int Texture::numFaces() const
{
    return m_numFaces;
}

gl::GLenum Texture::type() const
{
    return m_type;
}

//Surface & Texture::surface(unsigned int face)
//{
//    Assert(face < numFaces(), "");
//    return m_surfaces[face];
//}
//
//const Surface & Texture::surface(unsigned int face) const
//{
//    Assert(face < numFaces(), "");
//    return m_surfaces[face];
//}

Texture::Texture(gl::GLuint glName,
                 unsigned int width,
                 unsigned int height,
                 unsigned int numFaces
                 /*const std::vector<Surface> & surfaces*/):
    m_glName(glName),
    m_width(width),
    m_height(height),
    m_numFaces(numFaces)/*,
    m_surfaces(surfaces)*/
{
    if (m_numFaces == 1)
    {
        m_type = gl::GL_TEXTURE_2D;
    }
    else
    {
        Assert(m_numFaces == 6, "");
        m_type = gl::GL_TEXTURE_CUBE_MAP;
    }

}

}
