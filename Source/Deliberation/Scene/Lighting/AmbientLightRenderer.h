#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
class AmbientLightRenderer : public Renderer
{
public:
    AmbientLightRenderer();

    const glm::vec3 & color() const { return m_color; }

    void setColor(const glm::vec3 & color) { m_color = color; }

    void onRegisterRenderNodes() override;

private:
    friend class AmbientLightNode;

private:
    glm::vec3 m_color;
};
}