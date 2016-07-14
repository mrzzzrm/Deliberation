#include <Deliberation/Core/DataLayout.h>

#include <sstream>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

DataLayout::DataLayout(const std::vector<DataLayoutField::Desc> & descs)
{
    m_fields.reserve(descs.size());

    for (auto & desc : descs)
    {
        m_fields.push_back(DataLayoutField(desc.name, desc.type, m_stride));
        m_stride += desc.type.size();
    }
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

std::string DataLayout::toString()
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

