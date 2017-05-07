#pragma once

#include <functional>
#include <string>
#include <vector>

#include <Deliberation/Core/Detail/NamedDataMemberOf.h>
#include <Deliberation/Core/DataLayoutField.h>



namespace deliberation
{

class DataLayout final
{
public:
    template<typename T>
    static DataLayout fromStructOrClass(const std::vector<detail::NamedDataMemberOf<T>> & members);

    static DataLayout concatenate(const std::vector<DataLayout> & layouts);

public:
    DataLayout();
    DataLayout(const std::string & name, const Type & type, size_t arraySize = 1);
    DataLayout(const std::vector<DataLayoutField::Desc> & descs);
    DataLayout(std::vector<DataLayoutField> && fields, size_t stride);

    const std::vector<DataLayoutField> & fields() const;
    size_t stride() const;

    bool hasField(const std::string & name) const;

    const DataLayoutField & field(size_t index) const;
    const DataLayoutField & field(const std::string & name) const;

    void addField(const DataLayoutField & field);
    DataLayoutField addField(const DataLayoutField::Desc & desc);

    bool equals(const DataLayout & rhs) const;

    std::vector<DataLayoutField::Desc> descs() const;

    std::string toString() const;

private:
    std::vector<DataLayoutField> m_fields;
    size_t                       m_stride = 0u;
};

}

#include <Deliberation/Core/DataLayout.inl>