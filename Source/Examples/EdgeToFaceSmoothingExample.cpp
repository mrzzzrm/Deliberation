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
#include <Deliberation/Scene/Debug/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/EdgeToFaceSmoothing.h>

using namespace deliberation;

class EdgeToFaceSmoothingExample:
    public Application
{
public:
    EdgeToFaceSmoothingExample():
        Application("EdgeToFaceSmoothingExample")
    {

    }

    virtual void onStartup() override
    {
        m_program = context().createProgram({dataPath("Data/Examples/EdgeToFaceSmoothingExample.vert"),
                                             dataPath("Data/Examples/EdgeToFaceSmoothingExample.frag")});

        m_grid.reset(context(), 0.5f, m_camera);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(640.0f/480.0f);

        auto mesh = CuboidMesh({2.0f, 1.0f, 1.0f}).generate();
        auto smoothedMesh = EdgeToFaceSmoothing(mesh).run();
        auto sphereMesh = UVSphere(10, 10).generateMesh();

        m_unsmoothedCubeDraw = createDraw(mesh);
        m_smoothedCubeDraw = createDraw(smoothedMesh);
        m_sphereDraw = createDraw(sphereMesh);
        m_clear = context().createClear();

        m_navigator.reset(m_camera, input(), 1.0f);

        m_transform[0].setPosition({3.0f, 0.0f, 0.0f});
        m_transform[1].setPosition({-3.0f, 0.0f, 0.0f});
        m_transform[2].setPosition({0.0f, 0.0f, -3.0f});
    }

    Draw createDraw(const Mesh & mesh)
    {
        auto compilation = MeshCompiler().compile(mesh);

        auto draw = context().createDraw(m_program, gl::GL_TRIANGLES);
        draw.addVertices(compilation.vertices.layout(), compilation.vertices.rawData());
        draw.setIndices32(compilation.indices.rawData());
        draw.uniform("LightDirection").set(glm::normalize(glm::vec3(0, -1, -0.2)));

        return draw;
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        auto r = glm::quat({glm::pi<float>() * 0.3f * seconds,
                              glm::pi<float>() * 0.2f * seconds,
                              glm::pi<float>() * 0.1f * seconds});

        m_transform[0].localRotate(r);
        m_transform[1].worldRotate(r);

        m_unsmoothedCubeDraw.uniform("View").set(m_camera.view());
        m_unsmoothedCubeDraw.uniform("Projection").set(m_camera.projection());
        m_unsmoothedCubeDraw.uniform("Transform").set(m_transform[0].matrix());

        m_smoothedCubeDraw.uniform("View").set(m_camera.view());
        m_smoothedCubeDraw.uniform("Projection").set(m_camera.projection());
        m_smoothedCubeDraw.uniform("Transform").set(m_transform[1].matrix());

        m_sphereDraw.uniform("View").set(m_camera.view());
        m_sphereDraw.uniform("Projection").set(m_camera.projection());
        m_sphereDraw.uniform("Transform").set(m_transform[2].matrix());

        m_clear.schedule();
        m_unsmoothedCubeDraw.schedule();
        m_smoothedCubeDraw.schedule();
        m_sphereDraw.schedule();
        m_grid.get().draw();
    }

private:
    Draw                                m_unsmoothedCubeDraw;
    Draw                                m_smoothedCubeDraw;
    Draw                                m_sphereDraw;
    Program                             m_program;
    Clear                               m_clear;
    Optional<DebugGrid3DRenderer>       m_grid;
    Camera3D                            m_camera;
    Transform3D                         m_transform[3];
    Optional<DebugCameraNavigator3D>    m_navigator;

};

int main(int argc, char * argv[])
{
    return EdgeToFaceSmoothingExample().run();
}

