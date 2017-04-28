#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/LayoutedBlob.h>



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
    Uniform();

    template<typename T>
    void set(const T & value);

    template<typename T>
    void set(const std::vector<T> & vec);

    void set(const LayoutedBlob & array);

private:
    friend class Draw;

private:
    Uniform(detail::UniformImpl & impl);

private:
    detail::UniformImpl * m_impl;
};

}

#include <Deliberation/Draw/Uniform.inl>
