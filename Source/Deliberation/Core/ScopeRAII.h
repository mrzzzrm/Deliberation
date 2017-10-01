#pragma once

namespace deliberation
{

class ScopeRAII final {
public:
    ScopeRAII(const char * name);
    ~ScopeRAII();
};

}