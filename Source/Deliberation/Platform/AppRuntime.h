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
    AppRuntime(const std::string & name,
                       const std::string & prefix,
                       const std::string & entityPrototypeListPath);
    virtual ~AppRuntime() = default;

    const std::string & name() const { return m_name; }
    const std::string & prefix() const { return m_prefix; }
    const std::shared_ptr<World> & world() { return m_world; }
    const std::shared_ptr<EntityPrototypeManager> & entityPrototypeManager() { return m_entityPrototypeManager; }
    const std::shared_ptr<EventDomain> & events() const { return m_events; }
    const std::shared_ptr<PrototypeManager> & prototypeManager() const { return m_prototypeManager; }
    const std::shared_ptr<ResourceManager> & resourceManager() const { return m_resourceManager; }

    void setPrefix(const std::string & prefix) { m_prefix = prefix; }

    virtual void onStartup() {}
    virtual void onShutdown() {}
    virtual void onFrame(DurationMicros micros) {}

protected:
    std::string m_name;
    std::string m_prefix;

    std::shared_ptr<World> m_world;
    std::shared_ptr<EntityPrototypeManager> m_entityPrototypeManager;
    std::shared_ptr<EventDomain> m_events;
    std::shared_ptr<PrototypeManager> m_prototypeManager;
    std::shared_ptr<ResourceManager> m_resourceManager;
};

}