#pragma once

#include <stdlib.h>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API Span
{
public:
    template<size_t N>
    Span(T (&values)[N]);

    Span(std::vector<T> & vec);

    size_t size() const;

    std::vector<T> toVector() const;

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

private:
    T *     m_ptr;
    size_t  m_size;
};

}

#include <Deliberation/Core/Span.inl>