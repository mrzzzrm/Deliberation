#include <Deliberation/Scene/RenderPipeline.h>

#include <Deliberation/Scene/RenderNode.h>

namespace deliberation
{

RenderPipeline::RenderPipeline()
{

}

void RenderPipeline::clear()
{
    m_nodes.clear();
}

void RenderPipeline::addNode(RenderNode & node)
{
    m_nodes.push_back(&node);
}

void RenderPipeline::execute()
{
    for (auto * node : m_nodes)
    {
        node->execute();
    }
}

}

