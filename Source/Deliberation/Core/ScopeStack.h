#pragma once

#include <cinttypes>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include <Deliberation/Core/Globals.h>

namespace deliberation
{

using ScopeId = size_t;

class ScopeStack final
{
public:
    ScopeStack();

    void pushScope(const char * name);
    void popScope();

    ScopeId currentScopeId() const;

    const std::string & currentScopeName() const;

private:
    std::unordered_map<ScopeId, std::string> m_nameByScopeId;
    std::vector<size_t> m_scopeIdStack;
};

}
