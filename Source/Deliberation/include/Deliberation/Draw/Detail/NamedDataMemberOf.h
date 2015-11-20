#pragma once

#include <memory>
#include <string>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/OffsetOf.h>

#include <Deliberation/Draw/GL/GLSLTypeOf.h>
#include <Deliberation/Draw/Detail/INamedDataMemberOfImpl.h>

namespace deliberation
{

namespace detail
{

template<typename T>
class DELIBERATION_API NamedDataMemberOf final
{
public:
    template<typename U>
    NamedDataMemberOf(const std::string & name, U T::* ptr);

    const std::string & name() const;
    gl::GLenum type() const;
    unsigned int offset(const T & dummy) const;

private:
    std::shared_ptr<INamedDataMemberOfImpl<T>> m_impl;
    std::string m_name;
};

}

}

#include "NamedDataMemberOf.inl"
