#include <functional>
#include <sstream>

#include <Deliberation/Core/Assert.h>

#include <iostream>

namespace deliberation
{

template<typename T>
template<typename QVecT, typename QValT>
SparseVector<T>::IteratorT<QVecT, QValT>::IteratorT(QVecT & vec, std::size_t index):
    m_vec(vec),
    m_index(index)
{
    while (m_index < m_vec.m_vec.size() && !m_vec.contains(m_index))
    {
        m_index++;
    }
}

template<typename T>
template<typename QVecT, typename QValT>
template<typename OtherIteratorT>
SparseVector<T>::IteratorT<QVecT, QValT>::IteratorT(const OtherIteratorT & other):
    m_vec(other.m_vec),
    m_index(other.m_index)
{

}

template<typename T>
template<typename QVecT, typename QValT>
QValT & SparseVector<T>::IteratorT<QVecT, QValT>::operator*()
{
    return m_vec[m_index];
}

template<typename T>
template<typename QVecT, typename QValT>
SparseVector<T>::IteratorT<QVecT, QValT> & SparseVector<T>::IteratorT<QVecT, QValT>::operator++()
{
    do
    {
        m_index++;
    }
    while (m_index < m_vec.m_vec.size() && !m_vec.contains(m_index));

    return *this;
}

template<typename T>
template<typename QVecT, typename QValT>
bool SparseVector<T>::IteratorT<QVecT, QValT>::operator!=(const IteratorT<QVecT, QValT> & other) const
{
    return &m_vec != &other.m_vec || m_index != other.m_index;
}

template<typename T>
bool SparseVector<T>::contains(std::size_t index) const
{
    return index < m_vec.size() && m_vec[index].engaged();
}

template<typename T>
size_t SparseVector<T>::capacity() const
{
    return m_vec.size();
}

template<typename T>
size_t SparseVector<T>::count() const {
    return m_vec.size() - m_pool.size();
}

template<typename T>
bool SparseVector<T>::empty() const {
    return count() == 0;
}

template<typename T>
typename SparseVector<T>::Iterator SparseVector<T>::find(const T & value)
{
    for (std::size_t e = 0; e < m_vec.size(); e++)
    {
        auto & element = m_vec[e];
        if (element.engaged() && element.get() == value)
        {
            return Iterator(*this, e);
        }
    }

    return end();
}

template<typename T>
typename SparseVector<T>::CIterator SparseVector<T>::find(const T & value) const
{
    for (std::size_t e = 0; e < m_vec.size(); e++)
    {
        auto & element = m_vec[e];
        if (element.engaged() && element.get() == value)
        {
            return CIterator(*this, e);
        }
    }

    return end();
}

template<typename T>
std::size_t SparseVector<T>::insert(T && value)
{
    std::size_t result;

    if (m_pool.empty())
    {
        result = m_vec.size();
        m_vec.push_back(std::move(value));
    }
    else
    {
        result = m_pool.top();
        m_pool.pop();

        Assert(result < m_vec.size(), "");

        m_vec[result].reset(std::move(value));
    }

    return result;
}

template<typename T>
std::size_t SparseVector<T>::insert(const T & value)
{
    return insert(T(value));
}

template<typename T>
template<typename ... Args>
std::size_t SparseVector<T>::emplace(Args &&... args)
{
    std::size_t result;

    if (m_pool.empty())
    {
        result = m_vec.size();
        m_vec.emplace_back(std::forward<Args>(args)...);
    }
    else
    {
        result = m_pool.top();
        m_pool.pop();

        Assert(result < m_vec.size(), "");

        m_vec[result].reset(std::forward<Args>(args)...);
    }

    return result;
}

template<typename T>
void SparseVector<T>::erase(std::size_t index)
{
    Assert(index < m_vec.size(), "");
    Assert(m_vec[index].engaged(), "");

    m_vec[index].disengage();
    m_pool.push(index);
}

template<typename T>
typename SparseVector<T>::Iterator SparseVector<T>::erase(const Iterator & i)
{
    Iterator result = i;
    result++;

    erase(i.m_index);

    return result;
}

template<typename T>
void SparseVector<T>::clear()
{
    m_vec.clear();
    m_pool = decltype(m_pool)();
}

template<typename T>
typename SparseVector<T>::Iterator SparseVector<T>::begin()
{
    return Iterator(*this, 0);
}

template<typename T>
typename SparseVector<T>::Iterator SparseVector<T>::end()
{
    return Iterator(*this, m_vec.size());
}

template<typename T>
typename SparseVector<T>::CIterator SparseVector<T>::begin() const
{
    return CIterator(*this, 0);
}

template<typename T>
typename SparseVector<T>::CIterator SparseVector<T>::end() const
{
    return CIterator(*this, m_vec.size());
}

template<typename T>
T & SparseVector<T>::operator[](std::size_t index)
{
    Assert(contains(index), "");
    return m_vec[index].get();
}

template<typename T>
const T & SparseVector<T>::operator[](std::size_t index) const
{
    Assert(contains(index), "");
    return m_vec[index].get();
}

template<typename T>
std::string SparseVector<T>::toString() const
{
    std::stringstream stream;

    stream << "{";

    for (auto & o : m_vec)
    {
        if (!o.engaged())
        {
            continue;
        }

        stream << o.get() << "; ";
    }

    stream << "}";

    return stream.str();
}

}

