#pragma once

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API InputAdapter:
    public InputAdapterBase
{
public:
    void update();
};

}