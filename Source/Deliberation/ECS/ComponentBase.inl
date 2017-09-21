#include <Deliberation/ECS/AbstractWorld.h>

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

namespace deliberation
{
template<typename T>
void ComponentBase::publishEvent(const T & event)
{
    Application::get().runtime()->events()->publishEvent(event);
}
}