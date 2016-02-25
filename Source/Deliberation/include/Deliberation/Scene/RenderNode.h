#pragma once

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API RenderNode
{
public:
    RenderNode();
    virtual ~RenderNode();

    void execute();

protected:
    virtual void onExecute() = 0;
};

}

