#pragma once

#include <memory>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/Enum.h>

namespace deliberation
{
class SamplerImpl;
class Texture;

class Sampler final:
    public AbstractDrawObject<SamplerImpl>
{
public:
    Sampler() = default;

    void setTexture(const Texture & texture);

    void setWrap(TextureWrap wrap);
    void setWrapS(TextureWrap wrap);
    void setWrapT(TextureWrap wrap);
    void setWrapR(TextureWrap wrap);

    void setMinFilter(TextureFilter minFilter);
    void setMagFilter(TextureFilter magFilter);

private:
    friend class Draw;

private:
    Sampler(const std::shared_ptr<SamplerImpl> & impl);

    void setWrap(u32 index, TextureWrap wrap);
};
}
