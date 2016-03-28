namespace deliberation
{

namespace detail
{

inline EntityData::EntityData():
    id(-1)
{

}

inline EntityData::EntityData(Entity::id_t id, const std::string & name, Entity::id_t parent):
    id(id),
    name(name),
    parent(parent),
    children(),
    state(EntityState_None),
    transition(EntityTransition_None)
{

}

}

}

