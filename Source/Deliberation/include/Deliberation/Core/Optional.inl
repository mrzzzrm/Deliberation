#include <cassert>
#include <functional>

namespace deliberation
{

template<typename T>
Optional<T>::Optional():
    m_engaged(false)
{
}

template<typename T>
template<typename ... Args>
Optional<T>::Optional(Args && ... args):
    m_engaged(false)
{
    reset(std::forward<Args>(args)...);
}

template<typename T>
template<typename ... Args>
void Optional<T>::reset(Args && ... args)
{
    new(m_data) T(std::forward<Args>(args)...);
    m_engaged = true;
}

template<typename T>
bool Optional<T>::engaged() const
{
    return m_engaged;
}

template<typename T>
T & Optional<T>::get()
{
    assert(m_engaged);
    return *reinterpret_cast<T*>(m_data);
}

template<typename T>
const T & Optional<T>::get() const
{
    assert(m_engaged);
    return *reinterpret_cast<const T*>(m_data);
}


template<typename T>
T * Optional<T>::ptr()
{
    return m_engaged ? &get() : nullptr;
}

template<typename T>
const T * Optional<T>::ptr() const
{
    return m_engaged ? &get() : nullptr;
}

template<typename T>
template<typename ... Args>
void Optional<T>::reset(Args && ... args) const
{
    if (m_engaged)
    {
        disengage();
    }

    m_engaged = true;
    new(m_data) T(std::forward<Args>(args)...);
}

template<typename T>
void Optional<T>::disengage()
{
    if (!m_engaged)
    {
        return;
    }

    m_engaged = false;
    ((T*)m_data)->~T();
}

template<typename T>
T * Optional<T>::operator->()
{
    return ptr();
}

template<typename T>
const T * Optional<T>::operator->() const
{
    return ptr();
}

template<typename T>
bool Optional<T>::operator==(const T * other) const
{
    if (!other && !m_engaged)
    {
        return true;
    }

    if ((other && !m_engaged) || (!other && m_engaged))
    {
        return false;
    }

    return *other == get();
}

template<typename T>
bool Optional<T>::operator!=(const T * other) const
{
    return !operator==(other);
}

template<typename T>
bool Optional<T>::operator==(const T & other) const
{
    if (!m_engaged)
    {
        return false;
    }

    return *other == get();
}

template<typename T>
bool Optional<T>::operator!=(const T & other) const
{
    return !operator==(other);
}

}

