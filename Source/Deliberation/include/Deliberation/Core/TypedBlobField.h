#pragma once

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Blob;
class DataLayout;
class DataLayoutField;

template<typename T>
class TypedBlobField final
{
public:
    TypedBlobField(Blob & data, const DataLayout & layout, const DataLayoutField & field);

    void assign(const std::vector<T> & values);

    T & operator[](size_t index);
    const T & operator[](size_t index) const;

private:
    Blob &                  m_data;
    const DataLayout &      m_layout;
    const DataLayoutField & m_field;
};

}

#include <Deliberation/Core/TypedBlobField.inl>