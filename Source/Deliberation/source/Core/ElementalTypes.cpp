#include <Deliberation/Core/ElementalTypes.h>

namespace
{

using namespace deliberation;

struct ElementalTypeData
{
    const char * name;
    bool integer;
    bool signedInteger;
    u8 size;
};

ElementalTypeData ELEMENTAL_TYPE_DATA[] = {
    {"None", false, false, 0}, // None
    {"Bool", true, false, 1}, // Bool
    {"U8", true, false, 1}, // U8
    {"I8", true, true, 1},  // I8
    {"U16", true, false, 2}, // U16
    {"I16", true, true, 2},  // I16
    {"U32", true, false, 4}, // U32
    {"I32", true, true, 4},  // I32
    {"Float", false, false, 4}  // Float
};

}

namespace deliberation
{

ElementalType::ElementalType(u8 id):
    m_id(id)
{
}

u8 ElementalType::id() const
{
    return m_id;
}

const char * ElementalType::name() const
{
    return ELEMENTAL_TYPE_DATA[m_id].name;
}

u8 ElementalType::size() const
{
    return ELEMENTAL_TYPE_DATA[m_id].size;
}

bool ElementalType::isFloat() const
{
    return m_id == ElementalType_Float.m_id;
}

bool ElementalType::isInteger() const
{
    return ELEMENTAL_TYPE_DATA[m_id].integer;
}

bool ElementalType::isSignedInteger() const
{
    return ELEMENTAL_TYPE_DATA[m_id].signedInteger;
}

bool ElementalType::operator==(const ElementalType & other) const
{
    return m_id == other.m_id;
}

bool ElementalType::operator!=(const ElementalType & other) const
{
    return m_id != other.m_id;
}

ElementalType ElementalType_None(ELEMENTAL_TYPE_NONE);
ElementalType ElementalType_Bool(ELEMENTAL_TYPE_BOOL);
ElementalType ElementalType_U8(ELEMENTAL_TYPE_U8);
ElementalType ElementalType_U16(ELEMENTAL_TYPE_U16);
ElementalType ElementalType_U32(ELEMENTAL_TYPE_U32);
ElementalType ElementalType_I8(ELEMENTAL_TYPE_I8);
ElementalType ElementalType_I16(ELEMENTAL_TYPE_I16);
ElementalType ElementalType_I32(ELEMENTAL_TYPE_I32);
ElementalType ElementalType_Float(ELEMENTAL_TYPE_FLOAT);
}