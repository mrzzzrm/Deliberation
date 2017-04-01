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

enum TYPE {
    TYPE_NONE = 0,
    TYPE_I8 = 1,
    TYPE_I16 = 2,
    TYPE_I32 = 3,
    TYPE_U8 = 4,
    TYPE_U16 = 5,
    TYPE_U32 = 6,
    TYPE_FLOAT = 7,
    TYPE_DOUBLE = 8,
    TYPE_VEC2 = 9,
    TYPE_VEC3 = 10,
    TYPE_VEC4 = 11,
    TYPE_I8VEC2 = 12,
    TYPE_I8VEC3 = 13,
    TYPE_I8VEC4 = 14,
    TYPE_I16VEC2 = 15,
    TYPE_I16VEC3 = 16,
    TYPE_I16VEC4 = 17,
    TYPE_I32VEC2 = 18,
    TYPE_I32VEC3 = 19,
    TYPE_I32VEC4 = 20,
    TYPE_IVEC2 = TYPE_I32VEC2,
    TYPE_IVEC3 = TYPE_I32VEC3,
    TYPE_IVEC4 = TYPE_I32VEC4,
    TYPE_U8VEC2 = 21,
    TYPE_U8VEC3 = 22,
    TYPE_U8VEC4 = 23,
    TYPE_U16VEC2 = 24,
    TYPE_U16VEC3 = 25,
    TYPE_U16VEC4 = 26,
    TYPE_U32VEC2 = 27,
    TYPE_U32VEC3 = 28,
    TYPE_U32VEC4 = 29,
    TYPE_UVEC2 = TYPE_U32VEC2,
    TYPE_UVEC3 = TYPE_U32VEC3,
    TYPE_UVEC4 = TYPE_U32VEC4,
    TYPE_MAT2 = 30,
    TYPE_MAT3 = 31,
    TYPE_MAT4 = 32,
    TYPE_BOOL = 33
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