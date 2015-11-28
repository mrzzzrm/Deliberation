#pragma once

#include <memory>

namespace deliberation
{

namespace detail
{
    class UniformImpl;
}

class Draw;

class Uniform
{
public:
    template<typename T>
    void set(const T & value);

private:
    friend class Draw;

private:
    Uniform(detail::UniformImpl & impl);

private:
    detail::UniformImpl & m_impl;
};

}

#include <Deliberation/Draw/Uniform.inl>
