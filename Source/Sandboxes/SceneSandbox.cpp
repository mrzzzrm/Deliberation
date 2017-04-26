#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureLoader.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/Debug/DebugCubemapRenderer.h>
#include <Deliberation/Scene/Debug/DebugTexture2dRenderer.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>
#include <Deliberation/Scene/Model/Model.h>
#include <Deliberation/Scene/Model/ModelRenderer.h>
#include <Deliberation/Scene/Model/ModelInstance.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/HdrRenderer.h>
#include <Deliberation/Scene/SsaoRenderer.h>

using namespace deliberation;

class SceneSandbox:
    public Application
{
public:
    SceneSandbox():
        Application("SceneSandbox")
    {

    }

    virtual void onStartup() override
    {
        m_renderManager = std::make_shared<RenderManager>(drawContext());
        m_modelRenderer = m_renderManager->addRenderer<ModelRenderer>();
        m_ambientLightRenderer = m_renderManager->addRenderer<AmbientLightRenderer>();
        m_ssaoRenderer = m_renderManager->addRenderer<SsaoRenderer>();
        m_hdrRenderer = m_renderManager->addRenderer<HdrRenderer>();
        m_ground = m_renderManager->addRenderer<DebugGroundPlaneRenderer>();
        m_bunnyModel = m_modelRenderer->addModel(DeliberationDataPath("Data/Models/bunny.obj"));
        m_bunnyInstance = m_modelRenderer->addModelInstance(m_bunnyModel);
        m_bunnyInstance->transform.setPosition({0.0f, 0.32f, 0.0f});
       // EnableGLErrorChecksAndLogging();


        auto cubeModel = m_modelRenderer->addModel(DeliberationDataPath("Data/Models/cube.obj"));
        auto cubeInstance = m_modelRenderer->addModelInstance(cubeModel);
        cubeInstance->transform.setPosition({2.0f, 0.0f, 3.0f});

        m_ground->setRenderToGBuffer(true);
        m_ground->setSize(30.0f);
        m_ground->setRadius(30.0f);
        m_ground->setQuadSize(1.0f);

        m_renderManager->mainCamera().setPosition({0.0f, 1.0f, 3.0f});
        m_renderManager->mainCamera().setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_renderManager->mainCamera().setAspectRatio((float)drawContext().backbuffer().width() / drawContext().backbuffer().height());

        m_clear = drawContext().createClear();

        m_transform.setPosition({0.0f, 1.0f, 0.0f});
        m_navigator.reset(m_renderManager->mainCamera(), input(), 10.0f);

        auto skyboxPaths = std::array<std::string, 6> {
            DeliberationDataPath("Data/Skybox/Cloudy/Right.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Left.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Top.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Bottom.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Back.png"),
            DeliberationDataPath("Data/Skybox/Cloudy/Front.png")
        };

        auto faceTexture = drawContext().createTexture(
            TextureLoader(DeliberationDataPath("Data/Skybox/Debug/Right.png")).load());

        auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
        auto skyboxCubemap = drawContext().createTexture(skyboxCubemapBinary);

        m_skyboxRenderer.reset(drawContext(), m_renderManager->mainCamera(), skyboxCubemap);

        m_cubemapRenderer.reset(drawContext(), m_renderManager->mainCamera(), skyboxCubemap, DebugCubemapRenderer::MeshType::Sphere);
        m_cubemapRenderer->setPose(Pose3D::atPosition({10.0f, 10.0f, 0.0f}));
    }


    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        m_clear.render();

        m_cubemapRenderer->render();
        m_skyboxRenderer->render();

        m_renderManager->render();
    }

private:
    Draw                                m_draw;
    Clear                               m_clear;
    Transform3D                         m_transform;
    Optional<DebugCameraNavigator3D>    m_navigator;
    Optional<SkyboxRenderer>            m_skyboxRenderer;
    Optional<DebugCubemapRenderer>      m_cubemapRenderer;

    std::shared_ptr<RenderManager>              m_renderManager;
    std::shared_ptr<DebugGroundPlaneRenderer>   m_ground;
    std::shared_ptr<AmbientLightRenderer>       m_ambientLightRenderer;
    std::shared_ptr<HdrRenderer>                m_hdrRenderer;
    std::shared_ptr<SsaoRenderer>               m_ssaoRenderer;
    std::shared_ptr<ModelRenderer>              m_modelRenderer;
    std::shared_ptr<Model>                      m_bunnyModel;
    std::shared_ptr<ModelInstance>              m_bunnyInstance;
};

int main(int argc, char * argv[])
{
    return SceneSandbox().run(argc, argv);
}

