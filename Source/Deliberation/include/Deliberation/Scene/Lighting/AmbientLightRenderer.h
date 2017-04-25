#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{

class AmbientLightRenderer:
    public Renderer
{
public:
    AmbientLightRenderer(RenderManager & renderManager);

    const glm::vec3 & color() const { return m_color; }

    void setColor(const glm::vec3 & color) { m_color = color; }

    void render() override;

private:
    glm::vec3   m_color;
    Draw        m_draw;
};

}