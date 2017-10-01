#include <Deliberation/Core/ScopeStack.h>

#include <cinttypes>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Globals.h>
#include <Deliberation/Core/HashUtils.h>

namespace deliberation {

DELIBERATION_DEFINE_GLOBAL(ScopeStack)

void ScopeStack::pushScope(const char *name) {
    const auto id = reinterpret_cast<const uintptr_t>(static_cast<const void*>(name));

    auto iter = m_scopesById.find(id);
    if (iter == m_scopesById.end()) {
        iter = m_scopesById.emplace(id, std::make_shared<Scope>(name)).first;
    }

    m_scopeStack.emplace_back(iter->second);

    iter->second->begin();
}

void ScopeStack::popScope() {
    Assert(!m_scopeStack.empty());
    m_scopeStack.back()->end();
    m_scopeStack.pop_back();
}

}