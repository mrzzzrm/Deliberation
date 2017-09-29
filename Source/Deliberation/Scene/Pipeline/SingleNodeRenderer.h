#pragma once

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
class SingleNodeRenderer : public Renderer
{
public:
    SingleNodeRenderer(
        RenderPhase         renderPhase,
        const std::string & name = {"Unnamed Renderer"});

    void onRegisterRenderNodes() override;

    virtual void render() = 0;

protected:
    virtual void onSetupRender() {};

private:
    RenderPhase m_renderPhase;
};
}