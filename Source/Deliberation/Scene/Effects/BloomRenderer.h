#pragma once

#include <Deliberation/Scene/Effects/GaussianBlur.h>
#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class BloomRenderer : public SingleNodeRenderer
{
public:
    BloomRenderer(RenderManager & renderManager);

    void render() override;

    std::vector<Framebuffer> framebuffers() override { return {m_extractBuffer}; }

protected:
    void onSetupRender() override;

private:
    ScreenSpaceEffect m_extractEffect;
    Framebuffer       m_extractBuffer;
    GaussianBlur      m_blur;
    ScreenSpaceEffect m_applyEffect;
};

}