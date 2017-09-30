#include <Deliberation/Platform/AppRuntime.h>

#include <Deliberation/ECS/World.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/Resource/PrototypeManager.h>
#include <Deliberation/Resource/ResourceManager.h>

namespace deliberation
{

AppRuntime::AppRuntime(const std::string & entityPrototypeListPath)
{
    InitGlobal<World>();
    InitGlobal<EntityPrototypeManager>()->setPrototypeListPath(entityPrototypeListPath);
    InitGlobal<EventDomain>();
    InitGlobal<PrototypeManager>();
    InitGlobal<ResourceManager>();
}

}