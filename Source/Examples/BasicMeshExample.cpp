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
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class BasicMeshExample:
    public deliberation::Application
{
public:
    BasicMeshExample():
        deliberation::Application("BasicMeshExample")
    {

    }

    virtual void onStartup() override
    {
        m_grid.reset(context(), 0.5f, m_camera);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(640.0f/480.0f);

        m_draw = createDraw();
        m_clear = context().createClear();

        m_viewProjectionHandle = m_draw.uniform("ViewProjection");
        m_transformHandle = m_draw.uniform("Transform");

        m_navigator.reset(m_camera, inputAdapter(), 1.0f);
    }

    deliberation::Draw createDraw()
    {
        deliberation::VertexLayout layout({{"Position", deliberation::Type_Vec3},
                                           {"Color", deliberation::Type_Vec3}});
        deliberation::


        positions.add(glm::vec3(0, 0, 0));
        positions.add(glm::vec3(0, 0, -1));
        positions.add(glm::vec3(1, 0, -1));
        positions.add(glm::vec3(1, 0, 0));
        positions.add(glm::vec3(0, 1, 0));



        return deliberation::Draw();
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        m_transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * seconds,
                                           glm::pi<float>() * 0.2f * seconds,
                                           glm::pi<float>() * 0.1f * seconds}));

        m_viewProjectionHandle.set(m_camera.viewProjection());
        m_transformHandle.set(m_transform.matrix());

        m_clear.schedule();
        m_draw.schedule();
        m_grid.get().draw();
    }

private:
    deliberation::Draw                  m_draw;
    deliberation::Clear                 m_clear;
    deliberation::Optional<deliberation::DebugGrid3DRenderer>
                                        m_grid;
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
    return BasicMeshExample().run();
}

