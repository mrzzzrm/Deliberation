#pragma once

#include <Deliberation/Core/BlobValueAccessor.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Blob;
class DataLayout;
class DataLayoutField;

template<typename T>
class TypedBlobIterator final
{
public:
    TypedBlobIterator(T * ptr = nullptr, size_t stride = 0);

    inline void put(const T & value);

private:
    u8 *    m_ptr;
    size_t  m_stride;
};

template<typename T>
class CTypedBlobValueAccessor final
{
public:
    CTypedBlobValueAccessor() = default;
    CTypedBlobValueAccessor(const Blob & data, const DataLayout & layout, const DataLayoutField & field);

    const T & operator[](size_t index) const;

private:
    Optional<BlobValueAccessorData<const Blob>> m_data;
};

template<typename T>
class TypedBlobValueAccessor final
{
public:
    TypedBlobValueAccessor() = default;
    TypedBlobValueAccessor(Blob & data, const DataLayout & layout, const DataLayoutField & field);

    TypedBlobIterator<T> iterator();

    void assign(const std::vector<T> & values);

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

private:
    Optional<BlobValueAccessorData<Blob>> m_data;
};

}

#include <Deliberation/Core/TypedBlobValueAccessor.inl>