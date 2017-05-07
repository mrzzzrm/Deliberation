#pragma once

#include <stdlib.h>
#include <vector>

namespace deliberation
{
template<typename T>
class Span
{
  public:
    template<size_t N>
    Span(T (&values)[N]);

    Span(std::vector<T> & vec);

    Span(size_t size, T * ptr);

    size_t size() const;

    std::vector<T> toVector() const;

    T * begin() const;
    T * end() const;

    T & operator[](size_t index) const;

  private:
    T *    m_ptr;
    size_t m_size;
};
}

#include <Deliberation/Core/Span.inl>