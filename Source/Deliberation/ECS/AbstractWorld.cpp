#include <Deliberation/ECS/AbstractWorld.h>

namespace deliberation
{

AbstractWorld::AbstractWorld():
    m_events(std::make_shared<EventDomain>())
{

}

}