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
        Desc(const std::string & name, Type type);
        Desc(const Desc & desc) = default;

        Desc & operator=(const Desc & rhs) = default;
        Desc & operator=(Desc && rhs) = default;

        std::string name;
        Type        type = Type_None;
    };

public:
    DataLayoutField() = default;
    DataLayoutField(const std::string & name, Type type, size_t offset);

    const std::string & name() const;
    Type type() const;
    size_t offset() const;
    const Desc & desc() const;

    std::string toString() const;

private:
    Desc    m_desc;
    size_t  m_offset;
};

}