#include <Deliberation/Scene/Lighting/PointLightRenderer.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{

PointLightRenderer::PointLightRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::Lighting)
{
    m_lightLayout = DataLayout({
        {"LightPosition", Type_Vec3},
        {"Intensity", Type_Vec3},
        {"Active", Type_I32}
    });

    m_lightPositionField = m_lightLayout.field("LightPosition");
    m_intensityField = m_lightLayout.field("Intensity");
    m_activeField = m_lightLayout.field("Active");
}

// Reference invalidated by add/removePointLight()
PointLight & PointLightRenderer::pointLight(size_t index)
{
    Assert(index <= m_lights.size(), "Invalid index");
    return m_lights[index];
}

size_t PointLightRenderer::addPointLight(const PointLight & pointLight)
{
    if (m_pool.empty())
    {
        const auto numNewLights = 64;
        const auto indexBegin = m_lights.size();

        m_lights.resize(m_lights.size() + numNewLights);
        for (size_t i = 0; i < numNewLights; i++) m_pool.emplace(indexBegin + i);
    }

    size_t index = m_pool.top();
    m_pool.pop();

    m_lights[index] = pointLight;
    m_lights[index].active = true;

    return index;
}

void PointLightRenderer::removePointLight(size_t index)
{
    Assert(index <= m_lights.size(), "Invalid index");

    m_lights[index].active = false;

    m_pool.emplace(index);
}

void PointLightRenderer::render()
{
    if (m_lights.empty()) return;

    if (m_lightBuffer.count() != m_lights.size())
    {
        m_lightBuffer.reinit(m_lights.size());
    }

    auto lightData = m_lightBuffer.map(BufferMapping::WriteOnly);

    auto positions = lightData.iterator<glm::vec3>(m_lightPositionField);
    auto intensities = lightData.iterator<glm::vec3>(m_intensityField);
    auto active = lightData.iterator<i32>(m_activeField);

    for (size_t l = 0; l < m_lights.size(); l++)
    {
        positions.put(m_lights[l].position);
        intensities.put(m_lights[l].intensity);
        active.put(m_lights[l].active);
    }

    m_lightBuffer.unmap();

    m_viewProjectionUniform.set(renderManager().mainCamera().viewProjection());
    m_viewUniform.set(renderManager().mainCamera().view());

    m_draw.schedule();
}

void PointLightRenderer::onSetupRender()
{
    const auto mesh = UVSphere(10, 10).generateMesh2();

    auto program = drawContext().createProgram({
        DeliberationDataPath("Data/Shaders/PointLight.vert"),
        DeliberationDataPath("Data/Shaders/PointLight.frag")
    });

    m_lightBuffer = drawContext().createBuffer(m_lightLayout);
    m_draw = drawContext().createDraw(program);

    m_draw.addVertices(mesh.vertices());
    m_draw.setIndices(mesh.indices());

    m_draw.addInstanceBuffer(m_lightBuffer);

    m_draw.state().setDepthState(DepthState::disabledRW());
    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_ONE, gl::GL_ONE});
    m_draw.state().setCullState({gl::GL_FRONT});

    m_draw.setFramebuffer(renderManager().hdrBuffer());

    m_draw.sampler("Diffuse").setTexture(renderManager().gbuffer().colorTargetRef("Diffuse"));
    m_draw.sampler("Position").setTexture(renderManager().gbuffer().colorTargetRef("Position"));
    m_draw.sampler("Normal").setTexture(renderManager().gbuffer().colorTargetRef("Normal"));

    m_viewProjectionUniform = m_draw.uniform("ViewProjection");
    m_viewUniform = m_draw.uniform("View");
}

}