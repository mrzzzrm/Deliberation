#include <Deliberation/Core/ScopeStack.h>

#include <cinttypes>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Globals.h>
#include <Deliberation/Core/HashUtils.h>

namespace deliberation {

DELIBERATION_DEFINE_GLOBAL(ScopeStack)

ScopeStack::ScopeStack()
{
    pushScope("root");
}

void ScopeStack::pushScope(const char *name) {
    const auto intptr = static_cast<uintptr_t>(name);
    const auto prevScopeId = m_scopeIdStack.back();
    const auto currentScopeId = HashCombine(prevScopeId, intptr);
    m_scopeIdStack.emplace_back(currentScopeId);

    const auto iter = m_nameByScopeId.find(currentScopeId);
    if (iter == m_nameByScopeId.end()) {
        const auto iter2 = m_nameByScopeId.find(prevScopeId);
        Assert(iter2 != m_nameByScopeId.end());
        m_nameByScopeId.emplace(currentScopeId, iter2->second + ":" + name);
    }
}

void ScopeStack::popScope() {
    Assert(!m_scopeIdStack.empty());
    m_scopeIdStack.pop_back();
}

ScopeId ScopeStack::currentScopeId() const {
    Assert(!m_scopeIdStack.empty());
    return m_scopeIdStack.back();
}

const std::string &ScopeStack::currentScopeName() const {
    const auto iter = m_nameByScopeId.find(currentScopeId());
    Assert(iter != m_nameByScopeId.end());
    return iter->second;
}

}