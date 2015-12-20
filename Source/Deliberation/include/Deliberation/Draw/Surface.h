#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

namespace detail
{
    class Framebuffer;
    class TextureImpl;
}

class Context;

class DELIBERATION_API Surface final
{
public:
    Surface();

    Texture texture() const;
    unsigned int face() const;
    unsigned int width() const;
    unsigned int height() const;

private:
    friend class DrawOutput;
    friend class detail::Framebuffer;
    friend class detail::TextureImpl;

private:
    Surface(const std::shared_ptr<detail::TextureImpl> & texture,
            unsigned int face);

private:
    std::shared_ptr<detail::TextureImpl>    m_texture;
    unsigned int                            m_face;
};

}

