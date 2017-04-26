#pragma once

#include <stdlib.h>

#include <Deliberation/Core/BlobValue.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Blob;
class DataLayout;
class DataLayoutField;

template<typename BlobType>
struct BlobValueAccessorData final
{
    BlobValueAccessorData(BlobType & data,
                          const DataLayout & layout,
                          const DataLayoutField & field);

    BlobType &              data;
    const DataLayout &      layout;
    const DataLayoutField & field;
};

class CBlobValueAccessor final
{
public:
    CBlobValueAccessor() = default;
    CBlobValueAccessor(const Blob & data, const DataLayout & layout, const DataLayoutField & field);

    CBlobValue operator[](size_t index) const;

private:
    Optional<BlobValueAccessorData<const Blob>> m_data;
};

class BlobValueAccessor final
{
public:
    BlobValueAccessor() = default;
    BlobValueAccessor(Blob & data, const DataLayout & layout, const DataLayoutField & field);

    BlobValue operator[](size_t index);
    CBlobValue operator[](size_t index) const;

private:
    Optional<BlobValueAccessorData<Blob>> m_data;
};

}

#include <Deliberation/Core/BlobValueAccessor.inl>