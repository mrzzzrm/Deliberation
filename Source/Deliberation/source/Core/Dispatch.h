#pragma once

#include <vector>

#include <Deliberation/Core/Optional.h>



namespace deliberation
{

template<typename T>
class DoubleDispatch final
{
public:
    bool contains(size_t a, size_t b) const;

    void insert(size_t a, size_t b, const T & value);
    void insert(size_t a, size_t b, T && value);

    void erase(size_t a, size_t b);

    T & dispatch(size_t a, size_t b);
    const T & dispatch(size_t a, size_t b) const;

private:
    void sort(size_t & a, size_t & b) const;
    void ensureCapacity(size_t l, size_t r);

private:
    std::vector<std::vector<Optional<T>>> m_data;
};

}

#include <Deliberation/Core/Dispatch.inl>