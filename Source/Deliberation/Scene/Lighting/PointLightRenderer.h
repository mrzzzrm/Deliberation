#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

namespace deliberation
{

struct PointLight
{
    glm::vec3 position;
    glm::vec3 intensity;

    // To be populated by Renderer
    i32       active = false;

    PointLight() = default;

    PointLight(const glm::vec3 & position, const glm::vec3 intensity):
        position(position),
        intensity(intensity)
    {}
};

class PointLightRenderer:
    public SingleNodeRenderer
{
public:
    PointLightRenderer(RenderManager & renderManager);

    // Reference invalidated by add/removePointLight()
    PointLight & pointLight(size_t index);

    size_t addPointLight(const PointLight & pointLight);
    void removePointLight(size_t index);

    void render() override;

protected:
    void onSetupRender() override;

private:
    Draw                    m_draw;
    Buffer                  m_lightBuffer;
    DataLayout              m_lightLayout;
    LayoutedBlob            m_lightData;
    DataLayoutField         m_lightPositionField;
    DataLayoutField         m_intensityField;
    DataLayoutField         m_activeField;
    std::vector<PointLight> m_lights;
    std::stack<size_t>      m_pool;
    Uniform                 m_viewProjectionUniform;
    Uniform                 m_viewUniform;
};

}