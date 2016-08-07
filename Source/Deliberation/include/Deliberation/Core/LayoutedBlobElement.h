#pragma once

#include <stdlib.h>

#include <Deliberation/Core/BlobValue.h>

#include <Deliberation/Deliberation_API.h>
#include "DataLayoutField.h"

namespace deliberation
{

class Blob;
class DataLayout;
class LayoutedBlob;
class LayoutedBlobElement;

template<typename BlobType>
class DELIBERATION_API LayoutedBlobElementBase
{
public:
    using Base = LayoutedBlobElementBase<BlobType>;

public:
    BlobType & blob() const;
    const DataLayout & layout() const;
    size_t index() const;

    CBlobValue value(const DataLayoutField & field) const;
    CBlobValue value(const std::string & name) const;

    std::string toString() const;

protected:
    LayoutedBlobElementBase(BlobType & blob, const DataLayout & layout, size_t index);

protected:
    BlobType &          m_blob;
    const DataLayout &  m_layout;
    size_t              m_index = 0;
};

class DELIBERATION_API CLayoutedBlobElement final:
    public LayoutedBlobElementBase<const Blob>
{
public:
    CLayoutedBlobElement(const LayoutedBlobElement & rhs);

private:
    friend class LayoutedBlobElement;
    friend class LayoutedBlob;

private:
    CLayoutedBlobElement(const Blob & blob, const DataLayout & layout, size_t index);
};

class DELIBERATION_API LayoutedBlobElement final:
    public LayoutedBlobElementBase<Blob>
{
public:
    BlobValue value(const DataLayoutField & field);
    BlobValue value(const std::string & name);

    LayoutedBlobElement & operator=(const LayoutedBlobElement & rhs);
    LayoutedBlobElement & operator=(const CLayoutedBlobElement & rhs);

private:
    friend class LayoutedBlob;

private:
    LayoutedBlobElement(Blob & blob, const DataLayout & layout, size_t index);
};

}

#include <Deliberation/Core/LayoutedBlobElement.inl>