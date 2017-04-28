namespace deliberation
{

inline EntityData::EntityData():
    id(-1)
{

}

inline EntityData::EntityData(EntityId id, const std::string & name, EntityId parent):
    id(id),
    name(name),
    parent(parent),
    children(),
    componentSetup(nullptr)
{

}

}

