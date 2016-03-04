#include <functional>

namespace deliberation
{

namespace detail
{

template<typename T>
VectorBlobImpl<T>::VectorBlobImpl(std::size_t size):
    m_value(size)
{

}

template<typename T>
VectorBlobImpl<T>::VectorBlobImpl(const std::vector<T> & value):
    m_value(value)
{

}

template<typename T>
VectorBlobImpl<T>::VectorBlobImpl(std::vector<T> && value):
     m_value(std::move(value))
{

}

template<typename T>
std::unique_ptr<IBlobImpl> VectorBlobImpl<T>::clone() const
{
    return std::unique_ptr<IBlobImpl>(new VectorBlobImpl(m_value));
}

template<typename T>
const void * VectorBlobImpl<T>::ptr() const
{
    return m_value.data();
}

template<typename T>
std::size_t VectorBlobImpl<T>::size() const
{
    return m_value.size() * sizeof(T);
}

}

}

