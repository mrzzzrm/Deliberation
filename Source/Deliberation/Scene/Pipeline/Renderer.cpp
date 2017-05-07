#include <Deliberation/Scene/Pipeline/Renderer.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{
DrawContext & Renderer::drawContext() const
{
    return m_renderManager.drawContext();
}
}