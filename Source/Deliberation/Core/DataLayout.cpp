#include <Deliberation/Core/DataLayout.h>

#include <algorithm>
#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Types.h>

namespace deliberation
{
DataLayout DataLayout::concatenate(const std::vector<DataLayout> & layouts)
{
    std::vector<DataLayoutField::Desc> allDescs;
    for (auto & layout : layouts)
    {
        auto descs = layout.descs();
        allDescs.insert(allDescs.end(), descs.begin(), descs.end());
    }
    return DataLayout(allDescs);
}

DataLayout::DataLayout() {}

DataLayout::DataLayout(
    const std::string & name, const Type & type, size_t arraySize)
    : DataLayout({{name, type, arraySize}})
{
}

DataLayout::DataLayout(const std::vector<DataLayoutField::Desc> & descs)
{
    m_fields.reserve(descs.size());

    for (auto & desc : descs)
    {
        addField(desc);
    }
}

DataLayout::DataLayout(std::vector<DataLayoutField> && fields, size_t stride)
    : m_fields(std::move(fields)), m_stride(stride)
{
}

const std::vector<DataLayoutField> & DataLayout::fields() const
{
    return m_fields;
}

size_t DataLayout::stride() const { return m_stride; }

bool DataLayout::hasField(const std::string & name) const
{
    auto i = std::find_if(
        m_fields.begin(),
        m_fields.end(),
        [this, name](const DataLayoutField & field) {
            return field.name() == name;
        });
    return i != m_fields.end();
}

const DataLayoutField & DataLayout::field(size_t index) const
{
    Assert(index < m_fields.size(), "");
    return m_fields[index];
}

const DataLayoutField & DataLayout::field(const std::string & name) const
{
    for (auto & field : m_fields)
    {
        if (field.name() == name)
        {
            return field;
        }
    }

    Fail("No such field '" + name + "' in DataLayout");
}

void DataLayout::addField(const DataLayoutField & field)
{
    Assert(
        field.offset() >= m_stride,
        "Fields not added in offset-order, or fields are interleaving");

    m_fields.push_back(field);
    m_stride = field.offset() + field.type().size() * field.arraySize();
}

DataLayoutField DataLayout::addField(const DataLayoutField::Desc & desc)
{
    m_fields.push_back(
        DataLayoutField(desc.name, desc.type, m_stride, desc.arraySize));
    m_stride += desc.type.size() * desc.arraySize;

    return m_fields.back();
}

/**
 * TODO
 *  Just use simple hash
 */
bool DataLayout::equals(const DataLayout & rhs) const
{
    if (m_stride != rhs.m_stride || m_fields.size() != rhs.m_fields.size())
    {
        return false;
    }

    for (size_t f = 0; f < m_fields.size(); f++)
    {
        if (m_fields[f].type() != rhs.m_fields[f].type() ||
            m_fields[f].arraySize() != rhs.m_fields[f].arraySize())
        {
            return false;
        }
    }

    return true;
}

std::vector<DataLayoutField::Desc> DataLayout::descs() const
{
    std::vector<DataLayoutField::Desc> result;
    result.reserve(m_fields.size());
    for (auto & field : m_fields)
    {
        result.push_back(field.desc());
    }
    return result;
}

std::string DataLayout::toString() const
{
    std::stringstream stream;

    stream << "{Stride = " << m_stride << "; Fields={";

    for (auto & field : m_fields)
    {
        stream << field.toString() << ", ";
    }

    stream << "}}";

    return stream.str();
}
}
