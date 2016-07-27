#include <Deliberation/Core/DataLayout.h>

#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Types.h>

namespace deliberation
{

DataLayout::DataLayout()
{
}

DataLayout::DataLayout(const std::string & name, const Type & type):
    DataLayout({{name, type}})
{
}

DataLayout::DataLayout(const std::vector<DataLayoutField::Desc> & descs)
{
    m_fields.reserve(descs.size());

    for (auto & desc : descs)
    {
        m_fields.push_back(DataLayoutField(desc.name, desc.type, m_stride));
        m_stride += desc.type.size();
    }
}

DataLayout::DataLayout(std::vector<DataLayoutField> && fields, size_t stride):
    m_fields(std::move(fields)),
    m_stride(stride)
{
}

const std::vector<DataLayoutField> & DataLayout::fields() const
{
    return m_fields;
}

size_t DataLayout::stride() const
{
    return m_stride;
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
        if (m_fields[f].type() != rhs.m_fields[f].type())
        {
            return false;
        }
    }

    return true;
}

std::string DataLayout::toString() const
{
    std::stringstream stream;

    stream << "Layout: {Stride = " << m_stride << "; Fields={";

    for (auto & field : m_fields)
    {
        stream << field.toString() << ", ";
    }

    stream << "}}" << std::endl;

    return stream.str();
}

}

