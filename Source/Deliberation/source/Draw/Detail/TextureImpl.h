#pragma once

#include <glbinding/gl/types.h>

namespace deliberation
{

namespace detail
{

class TextureImpl final
{
public:
    TextureImpl(gl::GLuint glName,
                unsigned int width,
                unsigned int height,
                unsigned int numFaces/*,
                const std::vector<Surface> & surfaces*/);

    gl::GLuint glName;
    unsigned int width;
    unsigned int height;
    unsigned int numFaces;
    gl::GLenum type;
//    std::vector<Surface> m_surfaces;
};

}

}
