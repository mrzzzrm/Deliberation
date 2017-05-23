#pragma once

#include <limits>
#include <string>

#include <Deliberation/Core/Types.h>

namespace deliberation
{
class DataLayoutField final
{
  public:
    struct Desc
    {
        Desc() = default;
        Desc(const std::string & name, Type type, size_t arraySize = 1);
        Desc(const Desc & desc) = default;

        Desc & operator=(const Desc & rhs) = default;
        Desc & operator=(Desc && rhs) = default;

        bool operator==(const Desc & rhs) const;

        std::string name;
        Type        type = Type_None;
        size_t      arraySize = 0;
    };

  public:
    DataLayoutField() = default;
    DataLayoutField(
        const std::string & name,
        Type                type,
        size_t              offset,
        size_t              arraySize = 1);

    const std::string & name() const;
    Type                type() const;
    size_t              offset() const;
    size_t              arraySize() const { return m_desc.arraySize; }
    const Desc &        desc() const;

    std::string toString() const;

    bool operator==(const DataLayoutField & rhs) const;

  private:
    Desc   m_desc;
    size_t m_offset;
};
}