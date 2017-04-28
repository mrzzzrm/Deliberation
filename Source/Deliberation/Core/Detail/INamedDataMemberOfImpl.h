#pragma once

#include <Deliberation/Core/Types.h>



namespace deliberation
{

namespace detail
{

template<typename T>
class INamedDataMemberOfImpl
{
public:
    virtual ~INamedDataMemberOfImpl();

    virtual Type & type() const = 0;
    virtual unsigned int offset(const T & dummy) const = 0;
};

}

}

#include <Deliberation/Core/Detail/INamedDataMemberOfImpl.inl>
