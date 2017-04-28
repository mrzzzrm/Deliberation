#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

class DrawContext;

namespace detail
{

class TextureImpl final
{
public:
    static std::shared_ptr<TextureImpl> build(DrawContext & drawContext,
                                              unsigned int width,
                                              unsigned int height,
                                              unsigned int numFaces,
                                              PixelFormat format);

public:
    TextureImpl(DrawContext & drawContext, unsigned int numFaces);

    void allocate() const;
    void upload(const TextureBinary & binary);

    DrawContext &               drawContext;

    mutable gl::GLuint      glName;

    unsigned int            width;
    unsigned int            height;

    unsigned int            numFaces;
    gl::GLenum              type;
    PixelFormat             format;

    gl::GLuint              baseLevel;
    gl::GLuint              maxLevel;
    gl::GLenum              minFilter;
    gl::GLenum              maxFilter;

    std::vector<Surface>    surfaces;

    void bind() const;

private:
    void texImage2DAllFaces(const TextureBinary * binary) const;
};

}

}

