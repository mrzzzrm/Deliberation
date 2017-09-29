#pragma once

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
class HdrRenderer : public Renderer
{
public:
    HdrRenderer();

    void onRegisterRenderNodes() override;

private:
    friend class HdrNode;
};
}