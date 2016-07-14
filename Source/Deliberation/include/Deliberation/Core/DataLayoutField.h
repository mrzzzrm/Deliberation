#pragma once

#include <string>

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API DataLayoutField final
{
public:
    struct Desc
    {
        Desc(const std::string & name, const Type & type);

        std::string     name;
        const Type &    type;
    };

public:
    DataLayoutField(const std::string & name, const Type & type, size_t offset);

    const std::string & name() const;
    const Type & type() const;
    size_t offset() const;

    std::string toString() const;

private:
    std::string     m_name;
    const Type &    m_type;
    size_t          m_offset;
};

}