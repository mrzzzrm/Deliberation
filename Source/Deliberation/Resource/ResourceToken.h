#pragma once

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation {

using ResourceId = u32;

class ResourceToken {
public:
    ResourceToken(TypeID::value_t type, ResourceId id) :
        m_type(type),
        m_id(id) {}

    const TypeID::value_t type() const { return m_type; };

    const ResourceId id() const { return m_id; };

private:
    TypeID::value_t m_type;
    ResourceId m_id;
};

}