#pragma once

#include <memory>
#include <string>

namespace deliberation
{
namespace detail
{
    class IBlobImpl
    {
      public:
        virtual ~IBlobImpl() = default;
        virtual std::unique_ptr<IBlobImpl>
                             clone() const = 0; // TODO: With rvalue-ref-qualifier
        virtual const void * ptr() const = 0;
        virtual std::size_t  size() const = 0;
        virtual void         resize(std::size_t size) = 0;
        virtual void         reserve(std::size_t size) = 0;
    };
}
}
