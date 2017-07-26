#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/Enum.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Surface.h>

namespace deliberation
{
class TextureImpl;
class TextureBinary;
class Surface;

class Texture final:
    public AbstractDrawObject<TextureImpl>
{
public:
    Texture() = default;
    Texture(const std::shared_ptr<TextureImpl> & impl);
    Texture(const Surface & surface);

    unsigned int width() const;
    unsigned int height() const;

    TextureType type() const;

    PixelFormat format() const;

    unsigned int numFaces() const;

    Surface & surface(unsigned int face = 0);

    void upload(const TextureBinary & binary);

private:
    friend class DrawContext;
    friend class FramebufferImpl;
    friend class SurfaceDownloadImpl;
    friend class TextureUploadExecution;
    friend class Sampler;
    friend class Surface;
};
}
