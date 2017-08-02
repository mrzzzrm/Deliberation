#pragma once

#include <memory>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/Json.h>

namespace deliberation {

class PrototypeManager;

using PrototypeRevision = u32;

class AbstractPrototype {
public:
    explicit AbstractPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager) :
        m_prototypeManager(prototypeManager) {}

    PrototypeManager & prototypeManagerRef() const;
    std::shared_ptr<PrototypeManager> prototypeManager() const { return m_prototypeManager.lock(); }

    /**
     * For every reload, this is increased
     * @return
     */
    PrototypeRevision revision() const { return m_revision; }

    void reload(const Json &json);

protected:
    virtual void onReload(const Json &json) = 0;

private:
    std::weak_ptr<PrototypeManager> m_prototypeManager;
    PrototypeRevision               m_revision = 0;
};
}