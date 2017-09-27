#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/Phase.h>

#include <Deliberation/Platform/InputListener.h>
#include <Deliberation/Platform/InputLayerWrapper.h>

namespace deliberation
{
class ActivityManager;

class AbstractActivity : public InputListener
{
public:
    explicit AbstractActivity(const std::string & name):
    m_name(name)
    {}

    virtual ~AbstractActivity();

    // Internal - just use for debugging
    const std::vector<std::shared_ptr<AbstractPhaseInvoker>> & phaseInvokers() const { return m_phaseInvokers; }

    const Entity & entity() const { return m_entity; }
    const std::string & name() const { return m_name; }
    std::shared_ptr<ActivityManager> manager() const { return m_manager.lock(); }

    void setName(const std::string & name) { m_name = name; }

    void setEntity(const Entity & entity) {
        Assert(!m_entity.isValid());
        m_entity = entity;
        onAttachedToEntity();
    }

    void activateInputLayer(i32 inputPriority);
    void deactiveInputLayer();

    virtual void onCreated() {}
    virtual void onRemoved() {}

protected:
    virtual void onAttachedToEntity() {}

protected:
    std::vector<std::shared_ptr<AbstractPhaseInvoker>> m_phaseInvokers;
    std::string m_name;
    std::weak_ptr<ActivityManager> m_manager;
    std::shared_ptr<InputLayerWrapper> m_inputLayerWrapper;
    Entity m_entity;

private:
    friend class ActivityManager;
    friend class DebugOverlay;
};
}