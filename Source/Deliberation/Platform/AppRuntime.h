#pragma once

#include <memory>
#include <string>

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{

class EventDomain;
class EntityPrototypeManager;
class ResourceManager;
class PrototypeManager;
class World;

class AppRuntime
{
public:
    AppRuntime(const std::string & entityPrototypeListPath);
    virtual ~AppRuntime() = default;

    virtual void onStartup() {}
    virtual void onShutdown() {}
    virtual void onFrame(DurationMicros micros) {}
};

}