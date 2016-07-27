#pragma once

#include <string>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/TypedBlobField.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DataLayout;

class DELIBERATION_API LayoutedBlob final
{
public:
    LayoutedBlob(const DataLayout & layout, size_t count);

    const DataLayout & layout() const;
    size_t count() const;
    const Blob & rawData() const;

    template<typename T>
    TypedBlobField<T> field(const std::string & name);

    template<typename T>
    TypedBlobField<T> field(const DataLayoutField & field);

    template<typename T>
    void assign(const std::string & name, const std::vector<T> & values);

    std::string toString() const;

private:
    DataLayout m_layout;
    size_t     m_count;
    Blob       m_data;
};

}

#include <Deliberation/Core/LayoutedBlob.inl>