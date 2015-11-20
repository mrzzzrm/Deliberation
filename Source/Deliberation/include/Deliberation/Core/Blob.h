#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/Detail/IBlobImpl.h>

namespace deliberation
{
class Blob final
{
public:
    template<typename T>
    Blob(const std::vector<T> & value);

    template<typename T>
    Blob(std::vector<T> && value);

    Blob(const Blob & blob);

    Blob(Blob && blob);

    std::size_t size() const;
    const void * ptr() const;

    Blob & operator=(Blob && blob);

private:
    std::unique_ptr<detail::IBlobImpl> m_impl;
};

}

#include <Deliberation/Core/Blob.inl>
