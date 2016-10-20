#pragma once

#include <Deliberation/Core/Span.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
template<size_t N>
Span<T>::Span(T (&values)[N])
{
    m_ptr = values;
    m_size = N;
}

template<typename T>
Span<T>::Span(std::vector<T> & vec)
{
    m_ptr = vec.data();
    m_size = vec.size();
}

template<typename T>
Span<T>::Span(size_t size, T * ptr):
    m_size(size),
    m_ptr(ptr)
{

}

template<typename T>
size_t Span<T>::size() const
{
    return m_size;
}

template<typename T>
T * Span<T>::begin() const
{
    return m_ptr;
}

template<typename T>
T * Span<T>::end() const
{
    return &m_ptr[m_size];
}

template<typename T>
T & Span<T>::operator[](size_t index) const
{
    Assert(index < m_size, "");
    return m_ptr[index];
}

template<typename T>
std::vector<T> Span<T>::toVector() const
{
    std::vector<T> result;
    result.reserve(m_size);

    for (size_t i = 0; i < m_size; i++)
    {
        result.emplace_back(m_ptr[i]);
    }

    return result;
}

}