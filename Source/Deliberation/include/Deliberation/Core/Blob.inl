#include <Deliberation/Core/Detail/VectorBlobImpl.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
Blob Blob::fromValue(const T & value)
{
    Blob result;

    class Impl:
        public detail::IBlobImpl
    {
    public:
        Impl(const T & value):
            m_value(value)
        {

        }

        virtual std::unique_ptr<IBlobImpl> clone() const override
        {
            return std::unique_ptr<IBlobImpl>(new Impl(m_value));
        }

        virtual const void * ptr() const override
        {
            return &m_value;
        }

        virtual std::size_t size() const override
        {
            return sizeof(T);
        }

        void resize(std::size_t size) override
        {
            Fail("Resize not supported");
        }

        void reserve(std::size_t size) override
        {
            Fail("Reserve not supported");
        }

    private:
        T m_value;
    };

    result.m_impl = std::unique_ptr<detail::IBlobImpl>(new Impl(value));

    return result;
}

template<typename T>
Blob::Blob(const std::vector<T> & value)
{
    m_impl = std::unique_ptr<detail::IBlobImpl>(new detail::VectorBlobImpl<T>(value));
}

template<typename T>
Blob::Blob(std::vector<T> && value)
{
    m_impl = std::unique_ptr<detail::IBlobImpl>(new detail::VectorBlobImpl<T>(std::move(value)));
}

template<typename T>
void Blob::write(std::size_t offset, const T & value)
{
    Assert(offset + sizeof(T) <= size(), "");

    auto * bptr = (char*)ptr();
    auto * tptr = (T*)&bptr[offset];

    *tptr = value;
}

template<typename T>
void Blob::read(std::size_t offset, T & value) const
{
    Assert(offset + sizeof(T) <= size(), "");

    auto * bptr = (char*)ptr();
    auto * tptr = (T*)&bptr[offset];

    value = *tptr;
}

template<typename T>
T & Blob::access(size_t offset)
{
    Assert(offset + sizeof(T) <= size(), "");

    auto * bptr = (char*)ptr();
    return *(T*)&bptr[offset];
}

template<typename T>
const T & Blob::access(size_t offset) const
{
    Assert(offset + sizeof(T) <= size(), "");

    auto * bptr = (char*)ptr();
    return *(const T*)&bptr[offset];

}

}

