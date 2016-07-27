#pragma once

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{

template<typename T>
class DELIBERATION_API INamedDataMemberOfImpl
{
public:
    virtual ~INamedDataMemberOfImpl();

    virtual Type & type() const = 0;
    virtual unsigned int offset(const T & dummy) const = 0;
};

}

}

#include "INamedDataMemberOfImpl.inl"
