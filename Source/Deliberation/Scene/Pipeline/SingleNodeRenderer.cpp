#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>


namespace deliberation
{

SingleNodeRenderer::SingleNodeRenderer(
    RenderPhase         renderPhase,
    const std::string & name)
    : Renderer(name), m_renderPhase(renderPhase)
{
}

void SingleNodeRenderer::onRegisterRenderNodes()
{
    class SingleRenderNode : public RenderNode
    {
    public:
        SingleRenderNode(SingleNodeRenderer & renderer)
            : RenderNode(renderer.shared_from_this()), m_renderer(renderer)
        {
        }

        void        render() override { m_renderer.render(); }
        std::string name() const override { return m_renderer.name(); }

        SingleNodeRenderer & m_renderer;
    };

    GetGlobal<RenderManager>()->registerRenderNode(
        std::make_shared<SingleRenderNode>(*this), m_renderPhase);
    onSetupRender();
}

}