#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Detail/SurfaceImpl.h>
#include <Deliberation/Draw/Enum.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Surface.h>

namespace deliberation
{
class DrawContext;
class TextureBinary;

class TextureImpl final
{
public:
    TextureImpl(
        DrawContext & drawContext,
        u32           width,
        u32           height,
        u32           numFaces,
        PixelFormat   format);
    ~TextureImpl();

    void setupSurfaces(const std::shared_ptr<TextureImpl> & textureImpl);
    void texImage2DAllFaces(const TextureBinary * binary) const;

    DrawContext & drawContext;

    gl::GLuint glName;

    u32 width;
    u32 height;

    u32         numFaces;
    TextureType type;
    PixelFormat format;

    std::vector<Surface> surfaces;
};
}
