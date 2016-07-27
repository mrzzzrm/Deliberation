#pragma once

#include <string>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Type final
{
public:
    template<typename T>
    static constexpr Type & resolve();

public:
    Type();

    Type(const Type & type) = delete;
    Type(Type && type) = delete;

    Type(const std::string & name, size_t size, TypeID::value_t typeID);

    const std::string & name() const;
    size_t size() const;
    TypeID::value_t typeID() const;

    Type & operator=(const Type &) = delete;
    Type & operator=(Type &&) = delete;

    bool operator==(const Type & other);

private:
    std::string         m_name;
    size_t              m_size;
    TypeID::value_t     m_typeID;
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
extern Type Type_IVec2;
extern Type Type_IVec3;
extern Type Type_IVec4;
extern Type Type_UVec2;
extern Type Type_UVec3;
extern Type Type_UVec4;
extern Type Type_Mat2;
extern Type Type_Mat3;
extern Type Type_Mat4;

}

#include <Deliberation/Core/Types.inl>