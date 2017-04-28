#include <algorithm>
#include <functional>
#include <sstream>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::IteratorT(QualifiedRingBufferType & buffer, std::size_t index):
    m_buffer(buffer),
    m_index(index)
{

}

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
template<typename OtherIteratorT>
RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::IteratorT(const OtherIteratorT & other):
    m_buffer(other.m_buffer),
    m_index(other.m_index)
{

}

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
QualifiedT & RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::operator*()
{
    return m_buffer.at(m_index);
}

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
typename RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT> & RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::operator++()
{
    m_index++;
    return *this;
}

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
typename RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT> RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::operator++(int)
{
    auto result = *this;
    m_index++;
    return result;
}

template<typename T>
template<typename QualifiedRingBufferType, typename QualifiedT>
bool RingBuffer<T>::IteratorT<QualifiedRingBufferType, QualifiedT>::operator!=(const IteratorT & other)
{
    return &m_buffer != &other.m_buffer || m_index != other.m_index;
}

template<typename T>
RingBuffer<T>::RingBuffer():
    m_vec(),
    m_begin(0),
    m_size(0)
{

}

template<typename T>
RingBuffer<T>::RingBuffer(std::size_t capacity):
    m_vec(capacity, T{}),
    m_begin(0),
    m_size(0)
{

}

template<typename T>
bool RingBuffer<T>::empty() const
{
    return m_size == 0;
}

template<typename T>
T & RingBuffer<T>::front()
{
    return at(0);
}

template<typename T>
const T & RingBuffer<T>::front() const
{
    return at(0);
}

template<typename T>
T & RingBuffer<T>::back()
{
    Assert(m_size > 0, "");
    return at(m_size - 1);
}

template<typename T>
const T & RingBuffer<T>::back() const
{
    Assert(m_size > 0, "");
    return at(m_size - 1);
}

template<typename T>
T & RingBuffer<T>::at(std::size_t index)
{
    Assert(index < m_size, "");
    return m_vec[(m_begin + index) % m_vec.size()];
}

template<typename T>
const T & RingBuffer<T>::at(std::size_t index) const
{
    Assert(index < m_size, "");
    return m_vec[(m_begin + index) % m_vec.size()];
}

template<typename T>
std::size_t RingBuffer<T>::capacity() const
{
    return m_vec.size();
}

template<typename T>
std::size_t RingBuffer<T>::size() const
{
    return m_size;
}

template<typename T>
void RingBuffer<T>::push(T && value)
{
    auto index = (m_begin + m_size) % m_vec.size();
    m_vec[index] = std::forward<T>(value);

    if (m_size < capacity())
    {
        m_size++;
    }
    else
    {
        m_begin = (m_begin + 1) % capacity();
    }
}

template<typename T>
void RingBuffer<T>::pop()
{
    Assert(m_size > 0, "");
    m_size--;
}

template<typename T>
void RingBuffer<T>::reserve(std::size_t capacity)
{
    std::vector<T> newVec(capacity);

    std::size_t beginA = m_begin;
    std::size_t endA = m_begin + std::min(std::min(capacity, m_size), m_vec.size() - m_begin);
    std::copy(std::make_move_iterator(m_vec.begin() + beginA),
              std::make_move_iterator(m_vec.begin() + endA),
              newVec.begin());

    std::size_t sizeA = endA - beginA;
    std::size_t size = sizeA;

    if (sizeA < capacity && sizeA < m_size)
    {
        std::size_t endB = std::min(capacity - sizeA, m_size - sizeA);
        std::copy(std::make_move_iterator(m_vec.begin()),
                  std::make_move_iterator(m_vec.begin() + endB),
                  newVec.begin() + sizeA);
         size += endB;
    }

    m_vec = std::move(newVec);
    m_begin = 0;
    m_size = size;
}

template<typename T>
typename RingBuffer<T>::Iterator RingBuffer<T>::begin()
{
    return Iterator(*this, 0);
}

template<typename T>
typename RingBuffer<T>::Iterator RingBuffer<T>::end()
{
    return Iterator(*this, m_size);
}

template<typename T>
typename RingBuffer<T>::CIterator RingBuffer<T>::begin() const
{
    return CIterator(*this, 0);
}

template<typename T>
typename RingBuffer<T>::CIterator RingBuffer<T>::end() const
{
    return CIterator(*this, m_size);
}

template<typename T>
std::string RingBuffer<T>::toString() const
{
    std::stringstream stream;

    stream << "[Begin: " << m_begin << "; Size: " << size() << "; Capacity: " << capacity() << "; Elements: {";
    for (auto & value : *this)
    {
        stream << value << ", ";
    }
    stream << "}]";
    return stream.str();
}


}

