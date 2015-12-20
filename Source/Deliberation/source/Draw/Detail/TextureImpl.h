#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

class Context;

namespace detail
{

class TextureImpl final
{
public:
    static std::shared_ptr<TextureImpl> build(Context & context, unsigned int numFaces);

public:
    TextureImpl(Context & context, unsigned int numFaces);

    Context &               context;

    gl::GLuint              glName;

    unsigned int            width;
    unsigned int            height;

    unsigned int            numFaces;
    gl::GLenum              type;

    gl::GLuint              baseLevel;
    gl::GLuint              maxLevel;
    gl::GLenum              minFilter;
    gl::GLenum              maxFilter;

    std::vector<Surface>    surfaces;
};

}

}

