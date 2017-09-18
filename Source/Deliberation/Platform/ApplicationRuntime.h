#pragma once

#include <memory>
#include <string>

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{

class EntityPrototypeManager;
class World;

class ApplicationRuntime
{
public:
    ApplicationRuntime(const std::string & name, const std::string & prefix);
    virtual ~ApplicationRuntime() = default;

    const std::string & name() const { return m_name; }
    const std::string & prefix() const { return m_prefix; }
    const std::shared_ptr<World> & world() { return m_world; }
    const std::shared_ptr<EntityPrototypeManager> & entityPrototypeManager() { return m_entityPrototypeManager; }

    void setPrefix(const std::string & prefix) { m_prefix = prefix; }

    virtual void onStartup() {}
    virtual void onShutdown() {}
    virtual void onFrame(DurationMicros micros) {}

protected:
    std::string m_name;
    std::string m_prefix;

    std::shared_ptr<World> m_world;
    std::shared_ptr<EntityPrototypeManager> m_entityPrototypeManager;
};

}