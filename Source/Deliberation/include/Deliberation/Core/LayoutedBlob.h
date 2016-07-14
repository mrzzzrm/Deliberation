#pragma once

#include <string>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/TypedBlobField.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API LayoutedBlob final
{
public:
    LayoutedBlob(const DataLayout & layout, size_t count);

    const DataLayout & layout() const;
    size_t count() const;

    template<typename T>
    TypedBlobField<T> field(const DataLayoutField & field);

    std::string toString() const;

private:
    const DataLayout &  m_layout;
    size_t              m_count;
    Blob                m_data;
};

}

#include <Deliberation/Core/LayoutedBlob.inl>