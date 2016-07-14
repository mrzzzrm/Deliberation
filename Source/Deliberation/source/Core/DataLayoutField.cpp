#include <Deliberation/Core/DataLayoutField.h>

#include <sstream>

namespace deliberation
{

DataLayoutField::Desc::Desc(const std::string & name, const Type & type):
    name(name),
    type(type)
{

}

DataLayoutField::DataLayoutField(const std::string & name, const Type & type, size_t offset):
    m_name(name),
    m_type(type),
    m_offset(offset)
{

}

const std::string & DataLayoutField::name() const
{
    return m_name;
}

const Type & DataLayoutField::type() const
{
    return m_type;
}

size_t DataLayoutField::offset() const
{
    return m_offset;
}

std::string DataLayoutField::toString() const
{
    std::stringstream stream;

    stream << "[" << m_name << ": " << m_type.name() << " @ " << m_offset << "]";

    return stream.str();
}

}