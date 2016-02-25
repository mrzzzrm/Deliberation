#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class RenderNode;

class DELIBERATION_API RenderPipeline final
{
public:
    RenderPipeline();

    void clear();

    void addNode(RenderNode & node);

    void execute();

private:
    std::vector<RenderNode *> m_nodes;
};

}

