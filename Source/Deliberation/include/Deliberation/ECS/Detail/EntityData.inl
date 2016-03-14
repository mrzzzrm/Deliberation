namespace deliberation
{

namespace detail
{

inline EntityData::EntityData(Entity::id_t id, const std::string & name, Entity::id_t parent):
    id(id),
    name(name),
    parent(parent),
    children(),
    active(false),
    activationScheduled(false),
    deactivationScheduled(false),
    removalScheduled(false)
{

}

}

}

