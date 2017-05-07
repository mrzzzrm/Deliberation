#include <functional>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
namespace detail
{
    template<typename T>
    VectorBlobImpl<T>::VectorBlobImpl(std::size_t size) : m_value(size)
    {
    }

    template<typename T>
    VectorBlobImpl<T>::VectorBlobImpl(const std::vector<T> & value)
        : m_value(value)
    {
    }

    template<typename T>
    VectorBlobImpl<T>::VectorBlobImpl(std::vector<T> && value)
        : m_value(std::move(value))
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

    template<typename T>
    void VectorBlobImpl<T>::resize(std::size_t size)
    {
        auto num = size / sizeof(T);
        auto rem = size % sizeof(T);
        Assert(rem == 0, "Invalid resize");

        return m_value.resize(num);
    }

    template<typename T>
    void VectorBlobImpl<T>::reserve(std::size_t size)
    {
        auto num = size / sizeof(T);
        auto rem = size % sizeof(T);
        Assert(rem == 0, "Invalid reserve");

        return m_value.reserve(num);
    }
}
}
