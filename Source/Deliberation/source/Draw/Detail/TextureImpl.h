#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Surface.h>

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
                unsigned int numFaces);

    gl::GLuint glName;

    unsigned int width;
    unsigned int height;

    unsigned int numFaces;
    gl::GLenum type;

    gl::GLuint baseLevel;
    gl::GLuint maxLevel;
    gl::GLenum minFilter;
    gl::GLenum maxFilter;

    std::vector<Surface> surfaces;
};

}

}
