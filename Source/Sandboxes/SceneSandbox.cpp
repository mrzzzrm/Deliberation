#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>
#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugCubemapRenderer.h>
#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Debug/DebugTexture2dRenderer.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/HdrRenderer.h>
#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Model/Model.h>
#include <Deliberation/Scene/Model/ModelInstance.h>
#include <Deliberation/Scene/Model/ModelRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>
#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/SsaoRenderer.h>

using namespace deliberation;

class SceneSandbox : public Application
{
public:
    SceneSandbox() : Application("SceneSandbox") {}

    virtual void onStartup() override
    {
        // EnableGLErrorChecksAndLogging();

        m_world.addSystem<ApplicationSystem>(*this);
        m_renderSystem = m_world.addSystem<RenderSystem>();
        m_world.addSystem<ImGuiSystem>();
        auto & renderManager = m_renderSystem->renderManager();

        m_modelRenderer = renderManager.addRenderer<ModelRenderer>();
        m_ambientLightRenderer =
            renderManager.addRenderer<AmbientLightRenderer>();
        m_pointLightRenderer = renderManager.addRenderer<PointLightRenderer>();
        m_ssaoRenderer = renderManager.addRenderer<SsaoRenderer>();
        m_debugGeometryRenderer =
            renderManager.addRenderer<DebugGeometryRenderer>();
        m_hdrRenderer = renderManager.addRenderer<HdrRenderer>();
        m_ground = renderManager.addRenderer<DebugGroundPlaneRenderer>();
        renderManager.addRenderer<BloomRenderer>();
        m_world.addSystem<DebugPointLightSystem>(m_pointLightRenderer);

        m_bunnyModel = m_modelRenderer->addModel(
            DeliberationDataPath("Data/Models/bunny.obj"));
        m_bunnyInstance = m_modelRenderer->addModelInstance(m_bunnyModel);
        m_bunnyInstance->transform.setPosition({0.0f, 0.32f, 0.0f});
        // EnableGLErrorChecksAndLogging();

        auto cubeModel = m_modelRenderer->addModel(
            DeliberationDataPath("Data/Models/cube.obj"));
        auto cubeInstance = m_modelRenderer->addModelInstance(cubeModel);
        cubeInstance->transform.setPosition({2.0f, 0.0f, 3.0f});

        m_ground->setRenderToGBuffer(true);
        m_ground->setSize(30.0f);
        m_ground->setRadius(30.0f);
        m_ground->setQuadSize(1.0f);

        renderManager.mainCamera().setPosition({0.0f, 1.0f, 3.0f});
        renderManager.mainCamera().setOrientation(
            glm::quat({-0.2f, 0.0f, 0.0f}));
        renderManager.mainCamera().setAspectRatio(
            (float)drawContext().backbuffer().width() /
            drawContext().backbuffer().height());

        m_navigator.reset(renderManager.mainCamera(), input(), 10.0f);

        auto skyboxPaths = std::array<std::string, 6>{
            DeliberationDataPath("Data/Skybox/Cloudy/Right.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Left.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Top.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Bottom.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Back.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Front.png")};

        auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
        auto skyboxCubemap = drawContext().createTexture(skyboxCubemapBinary);

        renderManager.addRenderer<SkyboxRenderer>(skyboxCubemap);

        m_cubemapRenderer.reset(
            drawContext(),
            renderManager.mainCamera(),
            skyboxCubemap,
            DebugCubemapRenderer::MeshType::Sphere);
        m_cubemapRenderer->setPose(Pose3D::atPosition({10.0f, 10.0f, 0.0f}));

        // Init circle
        for (size_t p = 0; p < 6; p++)
        {
            auto index =
                m_pointLightRenderer->addPointLight({{}, {2.0f, 1.5f, 0.0f}});
            m_circlePointLights.emplace_back(index);
        }

        // Init static point lights
        auto numStaticPointLights = 15;
        auto staticPointLightsXDist = 4.0f;
        for (size_t p = 0; p < numStaticPointLights; p++)
        {
            glm::vec3 pos;
            pos.x = (-numStaticPointLights / 2.0f + p) * staticPointLightsXDist;
            pos.y = 0.05f + p * 0.05f;
            pos.z = -5.0f;

            m_pointLightRenderer->addPointLight({pos, {2.0f, 1.5f, 0.0f}});
        }
    }

    virtual void onFrame(const UpdateFrame & updateFrame) override
    {
//        m_world.frameBegin();
//        m_world.gameUpdate(updateFrame);

//        m_secondsAccum += seconds;

//        // Update circle
//        auto angle = m_secondsAccum;
//        auto radiansPerLight =
//            (2.0f * glm::pi<float>()) / m_circlePointLights.size();
//        auto radius = 3.0f + std::sin(m_secondsAccum);
//        for (size_t p = 0; p < m_circlePointLights.size(); p++)
//        {
//            auto & pointLight =
//                m_pointLightRenderer->pointLight(m_circlePointLights[p]);

//            glm::vec3 position;
//            pointLight.position.y = 1.0f + std::sin(m_secondsAccum) * 0.7f;

//            float lightAngle = angle + radiansPerLight * p;

//            pointLight.position.x = std::cos(lightAngle) * radius;
//            pointLight.position.z = std::sin(lightAngle) * radius;
//        }

//        m_navigator.get().update(seconds);

//        m_renderSystem->renderManager().render();
    }

    void onMouseMotion(MouseMotionEvent & event) override
    {
        m_navigator->onMouseMotion(event);
    }

private:
    World m_world;

    Optional<DebugCameraNavigator3D> m_navigator;
    Optional<DebugCubemapRenderer>   m_cubemapRenderer;
    std::vector<size_t>              m_circlePointLights;

    std::shared_ptr<RenderSystem>             m_renderSystem;
    std::shared_ptr<DebugGroundPlaneRenderer> m_ground;
    std::shared_ptr<DebugGeometryRenderer>    m_debugGeometryRenderer;
    std::shared_ptr<DebugGeometryNode>        m_debugGeometryNode;
    std::shared_ptr<PointLightRenderer>       m_pointLightRenderer;
    std::shared_ptr<AmbientLightRenderer>     m_ambientLightRenderer;
    std::shared_ptr<HdrRenderer>              m_hdrRenderer;
    std::shared_ptr<SsaoRenderer>             m_ssaoRenderer;
    std::shared_ptr<ModelRenderer>            m_modelRenderer;
    std::shared_ptr<Model>                    m_bunnyModel;
    std::shared_ptr<ModelInstance>            m_bunnyInstance;

    float m_secondsAccum =
        0.0f; // So, don't leave the Sandbox running for too long :P
};

int main(int argc, char * argv[]) { return SceneSandbox().run(argc, argv); }
