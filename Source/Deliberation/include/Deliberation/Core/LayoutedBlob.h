#pragma once

#include <string>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/BlobValueAccessor.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlobElement.h>
#include <Deliberation/Core/TypedBlobValueAccessor.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DataLayout;

class DELIBERATION_API LayoutedBlob final
{
public:
    LayoutedBlob();
    LayoutedBlob(const DataLayout & layout, size_t count);

    const DataLayout & layout() const;
    size_t count() const;
    const Blob & rawData() const;

    template<typename T>
    TypedBlobValueAccessor<T> field(const std::string & name);

    template<typename T>
    CTypedBlobValueAccessor<T> field(const std::string & name) const;

    template<typename T>
    TypedBlobValueAccessor<T> field(const DataLayoutField & field);

    template<typename T>
    CTypedBlobValueAccessor<T> field(const DataLayoutField & field) const;
   
    BlobValueAccessor field(const std::string & name);
    CBlobValueAccessor field(const std::string & name) const;

    BlobValueAccessor field(const DataLayoutField & field);
    CBlobValueAccessor field(const DataLayoutField & field) const;

    template<typename T>
    void assign(const std::string & name, const std::vector<T> & values);

    LayoutedBlobElement operator[](size_t index);
    CLayoutedBlobElement operator[](size_t index) const;

    std::string toString() const;

private:
    template<typename T> friend class LayoutedBlobElementBase;
    friend class CLayoutedBlobElement;
    friend class LayoutedBlobElement;

private:
    DataLayout m_layout;
    size_t     m_count = 0;
    Blob       m_data;
};

}

#include <Deliberation/Core/LayoutedBlob.inl>