#pragma once

#include <string>

#include <Deliberation/Core/ElementalTypes.h>
#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Type final
{
public:
    template<typename T>
    static constexpr Type & resolve();

public:
    Type(u8 id);
    Type(const Type & type) = default;

    const char * name() const;
    size_t size() const;
    u32 id() const;
    u8 numRows() const;
    u8 numColumns() const;
    ElementalType elementalType() const;

    std::string toString(const void * ptr) const;

    Type & operator=(const Type &) = default;
    Type & operator=(Type &&) = default;

    bool operator==(const Type & other) const;
    bool operator!=(const Type & other) const;

private:
    u8 m_id;
};

extern Type Type_None;
extern Type Type_I8;
extern Type Type_I16;
extern Type Type_I32;
extern Type Type_U8;
extern Type Type_U16;
extern Type Type_U32;
extern Type Type_Float;
extern Type Type_Double;
extern Type Type_Vec2;
extern Type Type_Vec3;
extern Type Type_Vec4;
extern Type Type_I8Vec2;
extern Type Type_I8Vec3;
extern Type Type_I8Vec4;
extern Type Type_I16Vec2;
extern Type Type_I16Vec3;
extern Type Type_I16Vec4;
extern Type Type_I32Vec2;
extern Type Type_I32Vec3;
extern Type Type_I32Vec4;
extern Type Type_IVec2;
extern Type Type_IVec3;
extern Type Type_IVec4;
extern Type Type_U8Vec2;
extern Type Type_U8Vec3;
extern Type Type_U8Vec4;
extern Type Type_U16Vec2;
extern Type Type_U16Vec3;
extern Type Type_U16Vec4;
extern Type Type_U32Vec2;
extern Type Type_U32Vec3;
extern Type Type_U32Vec4;
extern Type Type_UVec2;
extern Type Type_UVec3;
extern Type Type_UVec4;
extern Type Type_Mat2;
extern Type Type_Mat3;
extern Type Type_Mat4;
extern Type Type_Bool;

}

#include <Deliberation/Core/Types.inl>