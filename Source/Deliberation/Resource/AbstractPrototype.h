#pragma once

#include <memory>

#include <Deliberation/Core/Json.h>

namespace deliberation {

class PrototypeManager;

class AbstractPrototype {
public:
    AbstractPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager) :
        m_prototypeManager(prototypeManager) {}

    PrototypeManager & prototypeManagerRef() const;
    std::shared_ptr<PrototypeManager> prototypeManager() const { return m_prototypeManager.lock(); }

    virtual void reload(const Json &json) = 0;

private:
    std::weak_ptr<PrototypeManager> m_prototypeManager;
};
}