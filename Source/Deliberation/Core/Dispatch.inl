#include <Deliberation/Core/Dispatch.h>

#include <algorithm>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
template<typename T>
bool DoubleDispatch<T>::contains(size_t a, size_t b) const
{
    sort(a, b);

    if (a >= m_data.size())
    {
        return false;
    }

    if (b >= m_data[a].size())
    {
        return false;
    }

    return m_data[a][b].engaged();
}

template<typename T>
void DoubleDispatch<T>::insert(size_t a, size_t b, const T & value)
{
    sort(a, b);

    ensureCapacity(a, b);
    m_data[a][b].reset(value);
}

template<typename T>
void DoubleDispatch<T>::insert(size_t a, size_t b, T && value)
{
    sort(a, b);

    ensureCapacity(a, b);
    m_data[a][b].reset(std::move(value));
}

template<typename T>
void DoubleDispatch<T>::erase(size_t a, size_t b)
{
    sort(a, b);

    Assert(contains(a, b));

    m_data[a][b].disengage();
}

template<typename T>
T & DoubleDispatch<T>::dispatch(size_t a, size_t b)
{
    Assert(contains(a, b));

    sort(a, b);

    return m_data[a][b].get();
}

template<typename T>
const T & DoubleDispatch<T>::dispatch(size_t a, size_t b) const
{
    Assert(contains(a, b));

    sort(a, b);

    return m_data[a][b].get();
}

template<typename T>
void DoubleDispatch<T>::sort(size_t & a, size_t & b) const
{
    if (a > b)
    {
        std::swap(a, b);
    }
}

template<typename T>
void DoubleDispatch<T>::ensureCapacity(size_t l, size_t r)
{
    if (l >= m_data.size())
    {
        m_data.resize(l + 1);
    }

    if (r >= m_data[l].size())
    {
        m_data[l].resize(r + 1);
    }
}
}