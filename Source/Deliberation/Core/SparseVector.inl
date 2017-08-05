#include <functional>
#include <iostream>
#include <sstream>

#include <boost/optional.hpp>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
template<typename T>
template<typename QVecT, typename QValT>
SparseVector<T>::IteratorT<QVecT, QValT>::IteratorT(
    QVecT & vec, std::size_t index)
    : m_vec(vec), m_index(index)
{
    while (m_index < m_vec.m_vec.size() && !m_vec.contains(m_index))
    {
        m_index++;
    }
}

template<typename T>
template<typename QVecT, typename QValT>
template<typename OtherIteratorT>
SparseVector<T>::IteratorT<QVecT, QValT>::IteratorT(
    const OtherIteratorT & other)
    : m_vec(other.m_vec), m_index(other.m_index)
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
typename SparseVector<T>::template IteratorT<QVecT, QValT> &
    SparseVector<T>::IteratorT<QVecT, QValT>::operator++()
{
    do
    {
        m_index++;
    } while (m_index < m_vec.m_vec.size() && !m_vec.contains(m_index));

    return *this;
}

template<typename T>
template<typename QVecT, typename QValT>
bool SparseVector<T>::IteratorT<QVecT, QValT>::
     operator!=(const IteratorT<QVecT, QValT> & other) const
{
    return &m_vec != &other.m_vec || m_index != other.m_index;
}

template<typename T>
bool SparseVector<T>::contains(std::size_t index) const
{
    return index < m_vec.size() && m_vec[index];
}

template<typename T>
size_t SparseVector<T>::capacity() const
{
    return m_vec.size();
}

template<typename T>
size_t SparseVector<T>::count() const
{
    return m_count;
}

template<typename T>
bool SparseVector<T>::empty() const
{
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

    while (true)
    {
        if (m_pool.empty())
        {
            result = m_vec.size();
            m_vec.push_back(std::move(value));
            break;
        }
        else
        {
            result = m_pool.top();
            m_pool.pop();

            Assert(result < m_vec.size(), "");

            if (m_vec[result])
                continue; // Using an index without notifying the pool is
                          // possible

            m_vec[result] = std::move(value);

            break;
        }
    }

    incCount();

    return result;
}

template<typename T>
std::size_t SparseVector<T>::insert(const T & value)
{
    return insert(T(value));
}

template<typename T>
void SparseVector<T>::insert_at(size_t index, T && value)
{
    Assert(!contains(index), "Slot already taken");

    m_vec.reserve(index + 1);

    ensureSize(index);

    if (index >= m_vec.size())
        m_vec.emplace_back(boost::in_place_init, std::move(value));
    else
        m_vec[index].emplace(std::move(value));

    incCount();
}

template<typename T>
void SparseVector<T>::insert_at(size_t index, const T & value)
{
    insert_at(index, T(value));
}

template<typename T>
template<typename... Args>
std::size_t SparseVector<T>::emplace(Args &&... args)
{
    std::size_t result;

    while (true)
    {
        if (m_pool.empty())
        {
            result = m_vec.size();
            m_vec.emplace_back(T(std::forward<Args>(args)...));
            break;
        }
        else
        {
            result = m_pool.top();
            m_pool.pop();

            Assert(result < m_vec.size(), "");
            if (m_vec[result])
                continue; // Using an index without notifying the pool is
                          // possible

            m_vec[result] = T{std::forward<Args>(args)...};
            break;
        }
    }

    incCount();

    return result;
}

template<typename T>
template<typename... Args>
void SparseVector<T>::emplace_at(size_t index, Args &&... args)
{
    Assert(!contains(index), "Slot already taken");
    replace_at(index, std::forward<Args>(args)...);
}

template<typename T>
template<typename... Args>
void SparseVector<T>::replace_at(size_t index, Args &&... args)
{
    if (contains(index)) decCount();

    m_vec.reserve(index + 1);

    ensureSize(index);

    if (index >= m_vec.size())
        m_vec.emplace_back(T(std::forward<Args>(args)...));
    else
        m_vec[index].emplace(std::forward<Args>(args)...);

    incCount();
}

template<typename T>
void SparseVector<T>::erase(std::size_t index)
{
    Assert(contains(index), "");

    decCount();

    m_vec[index] = boost::optional<T>{};
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
    m_count = 0;
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
    return *m_vec[index];
}

template<typename T>
const T & SparseVector<T>::operator[](std::size_t index) const
{
    Assert(contains(index), "");
    return *m_vec[index];
}

template<typename T>
std::string SparseVector<T>::toString() const
{
    std::stringstream stream;

    stream << "{";

    for (auto & o : m_vec)
    {
        if (!o) continue;

        stream << *o << "; ";
    }

    stream << "}";

    return stream.str();
}

template<typename T>
void SparseVector<T>::ensureSize(size_t size)
{
    if (size <= m_vec.size()) return;

    for (size_t i = m_vec.size(); i < size; i++)
        m_pool.push(i);

    m_vec.resize(size);
}

template<typename T>
void SparseVector<T>::incCount()
{
    m_count++;
}

template<typename T>
void SparseVector<T>::decCount()
{
    Assert(m_count > 0, "");
    m_count--;
}
}
