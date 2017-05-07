#include <Deliberation/Core/DataLayoutField.h>

#include <sstream>

namespace deliberation
{
DataLayoutField::Desc::Desc(
    const std::string & name, Type type, size_t arraySize)
    : name(name), type(type), arraySize(arraySize)
{
}

DataLayoutField::DataLayoutField(
    const std::string & name, Type type, size_t offset, size_t arraySize)
    : m_desc({name, type, arraySize}), m_offset(offset)
{
}

const std::string & DataLayoutField::name() const { return m_desc.name; }

Type DataLayoutField::type() const { return m_desc.type; }

size_t DataLayoutField::offset() const { return m_offset; }

const DataLayoutField::Desc & DataLayoutField::desc() const { return m_desc; }

std::string DataLayoutField::toString() const
{
    std::stringstream stream;

    stream << "[" << m_desc.name << ": " << m_desc.type.name() << " @ "
           << m_offset << "]";

    return stream.str();
}
}