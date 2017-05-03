#pragma once

#include <memory>

#include <glbinding/gl/enum.h>

namespace deliberation
{

class SamplerImpl;
class Texture;

class Sampler final
{
public:
    Sampler() = default;

    void setTexture(const Texture & texture);

    void setWrap(gl::GLenum wrap);
    void setWrapS(gl::GLenum wrap);
    void setWrapT(gl::GLenum wrap);
    void setWrapR(gl::GLenum wrap);

private:
    friend class Draw;

private:
    Sampler(const std::shared_ptr<SamplerImpl> & impl);

private:
    std::shared_ptr<SamplerImpl> m_impl;
};

}

