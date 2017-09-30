#include <Deliberation/ECS/AbstractWorld.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

namespace deliberation
{
template<typename T>
void ComponentBase::publishEvent(const T & event)
{
    GetGlobal<EventDomain>()->publishEvent(event);
}
}