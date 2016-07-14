#pragma once

#include <string>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class Type final
{
public:
    Type(const std::string & name, size_t size, TypeID::value_t typeID);

    const std::string & name() const;
    size_t size() const;
    TypeID::value_t typeID() const;

private:
    std::string         m_name;
    size_t              m_size;
    TypeID::value_t     m_typeID;
};

extern Type Type_I32;
extern Type Type_Vec3;

}