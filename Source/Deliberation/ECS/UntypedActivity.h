#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Core/TypeID.h>
#include <Deliberation/ECS/Phase.h>
#include <Deliberation/Platform/InputListener.h>
#include <Deliberation/Platform/InputLayerWrapper.h>

namespace deliberation
{
class ActivityManager;

class UntypedActivity : public InputListener
{
public:
    explicit UntypedActivity(const std::string & name):
    m_name(name)
    {}

    virtual ~UntypedActivity();

    const std::string & name() const { return m_name; }

    void activateInputLayer(i32 inputPriority);
    void deactiveInputLayer();

protected:
    std::vector<std::shared_ptr<AbstractPhaseInvoker>> m_phaseInvokers;
    std::string m_name;
    std::weak_ptr<ActivityManager> m_manager;
    std::shared_ptr<InputLayerWrapper> m_inputLayerWrapper;

private:
    friend class ActivityManager;
};
}