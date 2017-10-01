#pragma once

#include <cinttypes>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Deliberation/Core/Globals.h>
#include <Deliberation/Core/Scope.h>

namespace deliberation
{

using ScopeId = uintptr_t;

class ScopeStack final
{
public:
    const std::unordered_map<ScopeId, std::shared_ptr<Scope>> & scopesById() const { return m_scopesById; }

    void pushScope(const char * name);
    void popScope();


private:
    std::unordered_map<ScopeId, std::shared_ptr<Scope>> m_scopesById;
    std::vector<std::shared_ptr<Scope>> m_scopeStack;
};

}

