#pragma once

#include <memory>

namespace deliberation
{

namespace detail
{

class IBlobImpl
{
public:
    virtual ~IBlobImpl() = default;
    virtual std::unique_ptr<IBlobImpl> clone() const = 0; // TODO: With rvalue-ref-qualifier
    virtual const void * ptr() const = 0;
    virtual std::size_t size() const = 0;
};

}

}

