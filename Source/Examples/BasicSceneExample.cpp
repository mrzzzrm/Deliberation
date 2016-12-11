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
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/MeshCompiler.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

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

        m_navigator.reset(m_camera, inputAdapter(), 1.0f);
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

        m_clear.schedule();
        m_draw.schedule();
        m_ground.get().schedule();
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

};

int main(int argc, char * argv[])
{
    return BasicSceneExample().run(argc, argv);
}

