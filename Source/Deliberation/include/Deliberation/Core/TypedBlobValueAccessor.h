#pragma once

#include <Deliberation/Core/BlobValueAccessor.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Blob;
class DataLayout;
class DataLayoutField;

template<typename T>
class CTypedBlobValueAccessor final:
    private BlobValueAccessorBase<const Blob>
{
public:
    CTypedBlobValueAccessor(const Blob & data, const DataLayout & layout, const DataLayoutField & field);

    const T & operator[](size_t index) const;
};

template<typename T>
class TypedBlobValueAccessor final:
    private BlobValueAccessorBase<Blob>
{
public:
    TypedBlobValueAccessor(Blob & data, const DataLayout & layout, const DataLayoutField & field);

    void assign(const std::vector<T> & values);

    T & operator[](size_t index);
    const T & operator[](size_t index) const;
};

}

#include <Deliberation/Core/TypedBlobValueAccessor.inl>