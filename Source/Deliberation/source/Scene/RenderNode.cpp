#include <Deliberation/Scene/RenderNode.h>

namespace deliberation
{

RenderNode::RenderNode() = default;

RenderNode::~RenderNode() = default;

void RenderNode::execute()
{
    onExecute();
}

}

