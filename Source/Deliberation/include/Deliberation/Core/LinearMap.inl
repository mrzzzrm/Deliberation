#include <iostream>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename Value>
LinearMap<Value>::Iterator::Iterator(LinearMap<Value> & map, std::size_t index):
    map(map)
{
    while (index < map.m_vec.size() && !map.m_vec[index].engaged())
    {
        index++;
    }

    if (index < map.m_vec.size())
    {
        pair.reset(std::make_pair(index, std::ref(map.m_vec[index].get())));
    }
}

template<typename Value>
LinearMap<Value>::Iterator::Iterator(const Iterator & other):
    map(other.map),
    pair(other.pair)
{

}

template<typename Value>
const std::pair<std::size_t, Value&> & LinearMap<Value>::Iterator::operator*()
{
    Assert(pair.engaged(), "");
    return pair.get();
}

template<typename Value>
typename LinearMap<Value>::Iterator & LinearMap<Value>::Iterator::operator++()
{
    Assert(pair.engaged(), "");

    auto index = pair.get().first;

    index++;
    while (index < map.m_vec.size() && !map.m_vec[index].engaged())
    {
        index++;
    }

    if (index < map.m_vec.size())
    {
        pair = std::make_pair(index, std::ref(map.m_vec[index].get()));
    }
    else
    {
        pair.disengage();
    }

    return *this;
}

template<typename Value>
bool LinearMap<Value>::Iterator::operator!=(const Iterator & other) const
{
    if (&map != &other.map)
    {
        return true;
    }
    if (!pair.engaged() && !other.pair.engaged())
    {
        return false;
    }
    if (pair.engaged() != other.pair.engaged())
    {
        return true;
    }
    return pair.get().first != other.pair.get().first;
}

template<typename Value>
LinearMap<Value>::CIterator::CIterator(const LinearMap<Value> & map, std::size_t index):
    map(map)
{
    while (index < map.m_vec.size() && !map.m_vec[index].engaged())
    {
        index++;
    }

    if (index < map.m_vec.size())
    {
        pair.reset(std::make_pair(index, std::ref(map.m_vec[index].get())));
    }
}

template<typename Value>
LinearMap<Value>::CIterator::CIterator(const CIterator & other):
    map(other.map),
    pair(other.pair)
{

}

template<typename Value>
const std::pair<std::size_t, const Value&> & LinearMap<Value>::CIterator::operator*()
{
    Assert(pair.engaged(), "");
    return pair.get();
}

template<typename Value>
typename LinearMap<Value>::CIterator & LinearMap<Value>::CIterator::operator++()
{
    Assert(pair.engaged(), "");

    auto index = pair.get().first;

    index++;
    while (index < map.m_vec.size() && !map.m_vec[index].engaged())
    {
        index++;
    }

    if (index < map.m_vec.size())
    {
        pair = std::make_pair(index, std::ref(map.m_vec[index].get()));
    }
    else
    {
        pair.disengage();
    }

    return *this;
}

template<typename Value>
bool LinearMap<Value>::CIterator::operator!=(const CIterator & other) const
{
    if (&map != &other.map)
    {
        return true;
    }
    if (!pair.engaged() && !other.pair.engaged())
    {
        return false;
    }
    if (pair.engaged() != other.pair.engaged())
    {
        return true;
    }
    return pair.get().first != other.pair.get().first;
}

template<typename Value>
bool LinearMap<Value>::contains(std::size_t key) const
{
    if (key >= m_vec.size())
    {
        return false;
    }
    return m_vec[key].engaged();
}

template<typename Value>
std::size_t LinearMap<Value>::keyUpperBound() const
{
    return m_vec.size();
}

template<typename Value>
Value & LinearMap<Value>::operator[](std::size_t key)
{
    if (key >= m_vec.size())
    {
        static bool warnedAboutSize = false;
        if (key > 4096 && !warnedAboutSize)
        {
            std::cout << "LinearMap: Excessive key: " << key << std::endl;
            warnedAboutSize = true;
        }

        m_vec.resize(key + 1);
    }

    if (!m_vec[key].engaged())
    {
        m_vec[key].reset();
    }

    return m_vec[key].get();
}

template<typename Value>
const Value & LinearMap<Value>::operator[](std::size_t key) const
{
    if (key >= m_vec.size())
    {
        Fail("Invalid element");
    }

    if (!m_vec[key].engaged())
    {
        Fail("Invalid element");
    }

    return m_vec[key].get();
}

template<typename Value>
typename LinearMap<Value>::Iterator LinearMap<Value>::begin()
{
    return Iterator(*this, 0);
}

template<typename Value>
typename LinearMap<Value>::Iterator LinearMap<Value>::end()
{
    return Iterator(*this, keyUpperBound());
}

template<typename Value>
typename LinearMap<Value>::CIterator LinearMap<Value>::begin() const
{
    return CIterator(*this, 0);
}

template<typename Value>
typename LinearMap<Value>::CIterator LinearMap<Value>::end() const
{
    return CIterator(*this, keyUpperBound());
}

template<typename Value>
void LinearMap<Value>::erase(const Iterator & i)
{
    auto index = i.pair.first;

    Assert(index < m_vec.size(), "");

    m_vec[index].disengage();
}

}

