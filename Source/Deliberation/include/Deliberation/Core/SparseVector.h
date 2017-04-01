#pragma once

#include <vector>
#include <stack>
#include <string>

#include <boost/optional.hpp>

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
    size_t count() const;

    bool empty() const;

    Iterator find(const T & value);
    CIterator find(const T & value) const;

    std::size_t insert(T && value);
    std::size_t insert(const T & value);

    void insert_at(size_t index, T && value);
    void insert_at(size_t index, const T & value);

    template<typename ... Args>
    std::size_t emplace(Args &&... args);

    template<typename ... Args>
    void emplace_at(size_t index, Args &&... args);

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
    void ensureSize(size_t size);
    void incCount();
    void decCount();

private:
    std::vector<boost::optional<T>>             m_vec;
    std::stack<std::size_t>                     m_pool;
    size_t                                      m_count = 0;
};

}

#include <Deliberation/Core/SparseVector.inl>
