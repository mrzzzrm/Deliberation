#pragma once

namespace deliberation
{

namespace detail
{
    class TextureBindingImpl;
}

class Texture;

class TextureBinding
{
public:
    TextureBinding();

    void set(const Texture & texture);

private:
    friend class Draw;

private:
    TextureBinding(detail::TextureBindingImpl & impl);

private:
    detail::TextureBindingImpl * m_impl;
};

}

