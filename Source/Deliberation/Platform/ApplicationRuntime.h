#pragma once

#include <string>

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{

class ApplicationRuntime
{
public:
    ApplicationRuntime(const std::string & name, const std::string & prefix);
    virtual ~ApplicationRuntime() = default;

    const std::string & name() { return m_name; }
    const std::string & prefix() { return m_prefix; }

    void setPrefix(const std::string & prefix) { m_prefix = prefix; }

    virtual void onStartup() {}
    virtual void onShutdown() {}
    virtual void onFrame(DurationMicros micros) {}

protected:
    std::string m_name;
    std::string m_prefix;
};

}