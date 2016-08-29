#pragma once

#include <vector>
#include <stack>
#include <string>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API SparseVector final
{
public:
    template<typename QVecT, typename QValT>
    class DELIBERATION_API IteratorT final
    {
    public:
        IteratorT(QVecT & vec, std::size_t index);

        template<typename OtherIteratorT>
        IteratorT(const OtherIteratorT & other);

        QValT & operator*();
        IteratorT<QVecT, QValT> & operator++();
        bool operator!=(const IteratorT<QVecT, QValT> & other) const;

    private:
        template<typename> friend class SparseVector;

    private:
        QVecT &     m_vec;
        std::size_t m_index;
    };

    typedef IteratorT<SparseVector, T> Iterator;
    typedef IteratorT<const SparseVector, const T> CIterator;

public:
    bool contains(std::size_t index) const;

    size_t capacity() const;

    Iterator find(const T & value);
    CIterator find(const T & value) const;

    std::size_t insert(T && value);
    std::size_t insert(const T & value);

    template<typename ... Args>
    std::size_t emplace(Args &&... args);

    void erase(std::size_t index);
    Iterator erase(const Iterator & i);

    void clear();

    Iterator begin();
    Iterator end();

    CIterator begin() const;
    CIterator end() const;

    T & operator[](std::size_t index);
    const T & operator[](std::size_t index) const;

    std::string toString() const;

private:
    template<typename, typename> friend class IteratorT;

private:
    std::vector<Optional<T>>    m_vec;
    std::stack<std::size_t>     m_pool;
};

}

#include <Deliberation/Core/SparseVector.inl>
