#pragma once

#include <functional>
#include <string>
#include <vector>

#include <Deliberation/Core/DataLayoutField.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API DataLayout final
{
public:
    DataLayout(const std::vector<DataLayoutField::Desc> & descs);

    const std::vector<DataLayoutField> & fields() const;
    size_t stride() const;

    const DataLayoutField & field(const std::string & name) const;

    std::string toString();

private:
    std::vector<DataLayoutField> m_fields;
    size_t                       m_stride = 0u;
};

}