#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Core/TypeID.h>
#include <Deliberation/ECS/Phase.h>

namespace deliberation
{
class ActivityManager;

class UntypedActivity
{
public:
    explicit UntypedActivity(const std::string & name):
    m_name(name)
    {}

    const std::string & name() const { return m_name; }

protected:
    std::vector<std::shared_ptr<AbstractPhaseInvoker>> m_phaseInvokers;
    std::string m_name;
    std::weak_ptr<ActivityManager> m_manager;

private:
    friend class ActivityManager;
};
}