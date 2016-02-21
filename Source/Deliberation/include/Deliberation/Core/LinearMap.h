#pragma once

#include <cstdlib>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

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
    bool contains(std::size_t key) const;

    std::size_t keyUpperBound() const;

    Value & operator[](std::size_t key);
    const Value & operator[](std::size_t key) const;

    Iterator begin();
    Iterator end();

    CIterator begin() const;
    CIterator end() const;

private:
    friend struct Iterator;
    friend struct CIterator;

private:
    std::vector<Optional<Value>> m_vec;
};

}

#include <Deliberation/Core/LinearMap.inl>
