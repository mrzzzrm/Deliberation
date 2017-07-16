#pragma once

#include <memory>
#include <string>

#include <Deliberation/Core/Detail/INamedDataMemberOfImpl.h>
#include <Deliberation/Core/OffsetOf.h>
#include <Deliberation/Core/Types.h>

namespace deliberation
{
namespace detail
{
    template<typename T>
    class NamedDataMemberOf final
    {
    public:
        template<typename U>
        NamedDataMemberOf(const std::string & name, U T::*ptr);

        const std::string & name() const;
        Type &              type() const;
        unsigned int        offset(const T & dummy) const;

    private:
        std::shared_ptr<INamedDataMemberOfImpl<T>> m_impl;
        std::string                                m_name;
    };
}
}

#include <Deliberation/Core/Detail/NamedDataMemberOf.inl>
