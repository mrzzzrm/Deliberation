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
template<typename ... Args>
void Optional<T>::reset(Args && ... args) const
{
    m_engaged = true;
    new(m_data) T(std::forward<Args>(args)...);
}

template<typename T>
void Optional<T>::disengage()
{

    m_engaged = false;
    ((T*)m_data)->~T();
}

}

