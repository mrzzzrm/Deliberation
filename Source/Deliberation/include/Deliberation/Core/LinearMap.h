#pragma once

#include <cstdlib>
#include <vector>

#include <boost/optional.hpp>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename Value>
class LinearMap final
{
public:
    struct Iterator
    {
        LinearMap<Value> &                       map;
        Optional<std::pair<std::size_t, Value&>> pair;

        Iterator(LinearMap<Value> & map, std::size_t index);
        Iterator(const Iterator & other);
        const std::pair<std::size_t, Value&> & operator*();
        Iterator & operator++();
        bool operator!=(const Iterator & other) const;
    };

    struct CIterator
    {
        const LinearMap<Value> &                       map;
        Optional<std::pair<std::size_t, const Value&>> pair;

        CIterator(const LinearMap<Value> & map, std::size_t index);
        CIterator(const CIterator & other);
        const std::pair<std::size_t, const Value&> & operator*();
        CIterator & operator++();
        bool operator!=(const CIterator & other) const;
    };

public:
    LinearMap();

    bool contains(std::size_t key) const;

    std::size_t keyUpperBound() const;

    std::size_t size() const;

    Value & operator[](std::size_t key);
    const Value & operator[](std::size_t key) const;

    Value & at(std::size_t key);
    const Value & at(std::size_t key) const;

    Iterator begin();
    Iterator end();

    CIterator begin() const;
    CIterator end() const;

    void erase(std::size_t key);
    Iterator erase(const Iterator & i);

    template<typename ... Args>
    std::pair<Iterator, bool> emplace(std::size_t key, Args &&... args);

private:
    friend struct Iterator;
    friend struct CIterator;

private:
    void ensureSize(std::size_t key);

private:
    /* Number of active keys */
    std::size_t                  m_size;

    std::vector<boost::optional<Value>>
                                m_vec;
};

}

#include <Deliberation/Core/LinearMap.inl>
