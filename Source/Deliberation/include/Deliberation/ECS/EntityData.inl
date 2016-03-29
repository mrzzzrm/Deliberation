namespace deliberation
{

inline EntityData::EntityData():
    id(-1)
{

}

inline EntityData::EntityData(entity_id_t id, const std::string & name, entity_id_t parent):
    id(id),
    name(name),
    parent(parent),
    children(),
    componentSetup(nullptr)
{

}

}

