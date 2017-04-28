#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{

enum ELEMENTAL_TYPE
{
    ELEMENTAL_TYPE_NONE,
    ELEMENTAL_TYPE_BOOL,
    ELEMENTAL_TYPE_U8,
    ELEMENTAL_TYPE_U16,
    ELEMENTAL_TYPE_U32,
    ELEMENTAL_TYPE_I8,
    ELEMENTAL_TYPE_I16,
    ELEMENTAL_TYPE_I32,
    ELEMENTAL_TYPE_FLOAT
};

class ElementalType final
{
public:
    ElementalType(u8 id);

    u8 id() const;

    const char * name() const;
    u8 size() const;
    bool isFloat() const;
    bool isInteger() const;
    bool isSignedInteger() const;

    bool operator==(const ElementalType & other) const;
    bool operator!=(const ElementalType & other) const;

private:
    u8 m_id;
};

extern ElementalType ElementalType_None;
extern ElementalType ElementalType_Bool;
extern ElementalType ElementalType_U8;
extern ElementalType ElementalType_U16;
extern ElementalType ElementalType_U32;
extern ElementalType ElementalType_I8;
extern ElementalType ElementalType_I16;
extern ElementalType ElementalType_I32;
extern ElementalType ElementalType_Float;

}
