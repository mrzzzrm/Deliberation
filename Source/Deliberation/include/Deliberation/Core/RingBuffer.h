#pragma once

#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API RingBuffer final
{
public:
    template<typename QualifiedRingBufferType, typename QualifiedT>
    class DELIBERATION_API IteratorT final
    {
    public:
        IteratorT(QualifiedRingBufferType & buffer, std::size_t index);

        template<typename OtherIteratorT>
        IteratorT(const OtherIteratorT & other);

        QualifiedT & operator*();
        IteratorT & operator++();
        IteratorT operator++(int);
        bool operator!=(const IteratorT & other);

    private:
        QualifiedRingBufferType &   m_buffer;
        std::size_t                 m_index;
    };

    typedef IteratorT<RingBuffer, T>              Iterator;
    typedef IteratorT<const RingBuffer, const T>  CIterator;

public:
    RingBuffer();
    RingBuffer(std::size_t capacity);

    T & front();
    const T & front() const;
    T & back();
    const T & back() const;

    T & at(std::size_t index);
    const T & at(std::size_t index) const;
    std::size_t capacity() const;
    std::size_t size() const;

    void push(T && value);
    void pop();

    Iterator begin();
    Iterator end();

    CIterator begin() const;
    CIterator end() const;

    std::string toString() const;

private:
    template<typename _T, typename _K> friend class IteratorT;

private:
    std::vector<T>  m_vec;
    std::size_t     m_begin;
    std::size_t     m_size;
};

}

#include <Deliberation/Core/RingBuffer.inl>
