#pragma once

#include <stdlib.h>

#include <Deliberation/Core/BlobValue.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Blob;
class DataLayout;
class DataLayoutField;

template<typename BlobType>
struct BlobValueAccessorBase
{
    BlobType &              m_data;
    const DataLayout &      m_layout;
    const DataLayoutField & m_field;
};

class DELIBERATION_API CBlobValueAccessor final:
    private BlobValueAccessorBase<const Blob>
{
public:
    CBlobValueAccessor(const Blob & data, const DataLayout & layout, const DataLayoutField & field);

    CBlobValue operator[](size_t index) const;
};

class DELIBERATION_API BlobValueAccessor final:
    private BlobValueAccessorBase<Blob>
{
public:
    BlobValueAccessor(Blob & data, const DataLayout & layout, const DataLayoutField & field);

    BlobValue operator[](size_t index);
    CBlobValue operator[](size_t index) const;
};

}