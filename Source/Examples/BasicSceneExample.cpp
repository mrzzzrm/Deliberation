#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureLoader.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/SkyboxRenderer.h>
#include <Deliberation/Scene/Debug/DebugCubemapRenderer.h>
#include <Deliberation/Scene/Debug/DebugTexture2dRenderer.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/MeshCompiler.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

using namespace deliberation;

class BasicSceneExample:
    public deliberation::Application
{
public:
    BasicSceneExample():
        deliberation::Application("BasicSceneExample")
    {

    }

    virtual void onStartup() override
    {
        m_ground.reset(context(), m_camera);
        m_ground.get().setSize(30.0f);
        m_ground.get().setRadius(10.0f);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_draw = createDraw();
        m_clear = context().createClear();

        m_transform.setPosition({0.0f, 1.0f, 0.0f});

        m_viewProjectionHandle = m_draw.uniform("ViewProjection");
        m_transformHandle = m_draw.uniform("Transform");

        m_navigator.reset(m_camera, input(), 10.0f);

        auto skyboxPaths = std::array<std::string, 6> {
//            deliberation::dataPath("Data/Skybox/Yokohama3/posx.png"),
//            deliberation::dataPath("Data/Skybox/Yokohama3/negx.png"),
//            deliberation::dataPath("Data/Skybox/Yokohama3/posy.png"),
//            deliberation::dataPath("Data/Skybox/Yokohama3/negy.png"),
//            deliberation::dataPath("Data/Skybox/Yokohama3/posz.png"),
//            deliberation::dataPath("Data/Skybox/Yokohama3/negz.png")
            deliberation::dataPath("Data/Skybox/Debug/Right.png"),
            deliberation::dataPath("Data/Skybox/Debug/Left.png"),
            deliberation::dataPath("Data/Skybox/Debug/Top.png"),
            deliberation::dataPath("Data/Skybox/Debug/Bottom.png"),
            deliberation::dataPath("Data/Skybox/Debug/Back.png"),
            deliberation::dataPath("Data/Skybox/Debug/Front.png")
//            deliberation::dataPath("Data/Skybox/skybox/right.jpg"),
//            deliberation::dataPath("Data/Skybox/skybox/left.jpg"),
//            deliberation::dataPath("Data/Skybox/skybox/top.jpg"),
//            deliberation::dataPath("Data/Skybox/skybox/bottom.jpg"),
//            deliberation::dataPath("Data/Skybox/skybox/back.jpg"),
//            deliberation::dataPath("Data/Skybox/skybox/front.jpg")
        };

        auto faceTexture = context().createTexture(
            TextureLoader(deliberation::dataPath("Data/Skybox/Debug/Right.png")).load());

        auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
        auto skyboxCubemap = context().createTexture(skyboxCubemapBinary);

        m_skyboxRenderer.reset(context(), m_camera, skyboxCubemap);

        m_cubemapRenderer.reset(context(), m_camera, skyboxCubemap, DebugCubemapRenderer::MeshType::Sphere);
        m_cubemapRenderer->setPose(Pose3D::atPosition({10.0f, 0.0f, 0.0f}));

        m_textureRenderer.reset(context(), faceTexture);
    }

    deliberation::Draw createDraw()
    {
        auto program = context().createProgram({
                                                 deliberation::dataPath("Data/Examples/BasicSceneExample.vert"),
                                                 deliberation::dataPath("Data/Examples/BasicSceneExample.frag")
                                             });

        deliberation::UVSphere sphere(7, 7);
        auto mesh = sphere.generateMesh();

        deliberation::MeshCompiler compiler;
        auto compilation = compiler.compile(mesh);

        auto draw = context().createDraw(program, gl::GL_TRIANGLES);
        draw.addVertices(compilation.vertices);
        draw.setIndices(compilation.indices);

        return draw;
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

//        m_transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * seconds,
//                                           glm::pi<float>() * 0.2f * seconds,
//                                           glm::pi<float>() * 0.1f * seconds}));

        m_viewProjectionHandle.set(m_camera.viewProjection());
        m_transformHandle.set(m_transform.matrix());

        m_clear.render();

  //      m_textureRenderer->render();
//
//        m_cubemapRenderer->render();

        m_skyboxRenderer->render();
//
        m_draw.schedule();
        m_ground.get().render();
    }

private:
    deliberation::Draw                  m_draw;
    deliberation::Clear                 m_clear;
    deliberation::Optional<deliberation::DebugGroundPlaneRenderer>
                                        m_ground;
    deliberation::Camera3D              m_camera;
    deliberation::Transform3D           m_transform;
    deliberation::Texture               m_depthBuffer;
    deliberation::Texture               m_colorBuffer;
    deliberation::Uniform               m_viewProjectionHandle;
    deliberation::Uniform               m_transformHandle;
    deliberation::Optional<deliberation::DebugCameraNavigator3D>
                                        m_navigator;
    deliberation::Optional<deliberation::SkyboxRenderer>
                                        m_skyboxRenderer;
    deliberation::Optional<deliberation::DebugCubemapRenderer>
                                        m_cubemapRenderer;
    deliberation::Optional<deliberation::DebugTexture2dRenderer>
                                        m_textureRenderer;

};

int main(int argc, char * argv[])
{
    return BasicSceneExample().run(argc, argv);
}

