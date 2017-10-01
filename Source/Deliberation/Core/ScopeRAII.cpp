#include <Deliberation/Core/ScopeRAII.h>

#include <Deliberation/Core/Globals.h>
#include <Deliberation/Core/ScopeStack.h>

namespace deliberation
{

ScopeRAII::ScopeRAII(const char * name) {
    GetGlobal<ScopeStack>()->pushScope(name);
}

ScopeRAII::~ScopeRAII() {
    GetGlobal<ScopeStack>()->popScope();
}

}