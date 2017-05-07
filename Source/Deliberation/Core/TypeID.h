#pragma once

#include <stdint.h>

namespace deliberation
{
class TypeID
{
  public:
    typedef uint32_t value_t;

  public:
    template<typename T>
    static value_t value();

    template<typename Family, typename T>
    static value_t value();

  private:
    template<typename Family, typename T>
    static value_t generate();
};
}

#include <Deliberation/Core/TypeID.inl>
