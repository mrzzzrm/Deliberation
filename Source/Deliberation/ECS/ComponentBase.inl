#include <Deliberation/ECS/AbstractWorld.h>

namespace deliberation
{

template<typename T>
void ComponentBase::publishEvent(const T & event)
{
    m_world->events()->publishEvent(event);
}

}