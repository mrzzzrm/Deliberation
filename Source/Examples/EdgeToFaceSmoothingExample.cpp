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
        m_program = context().createProgram({dataPath("Data/Examples/BasicSceneExample.vert"),
                                             dataPath("Data/Examples/BasicSceneExample.frag")});

        m_grid.reset(context(), 0.5f, m_camera);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(640.0f/480.0f);

        auto mesh = CuboidMesh({2.0f, 1.0f, 1.0f}).generate();
        auto smoothedMesh = EdgeToFaceSmoothing(mesh).run();

        m_unsmoothedCubeDraw = createDraw(mesh);
        m_smoothedCubeDraw = createDraw(smoothedMesh);
        m_clear = context().createClear();

        m_navigator.reset(m_camera, inputAdapter(), 1.0f);

        m_transform[0].setPosition({3.0f, 0.0f, 0.0f});
        m_transform[1].setPosition({-3.0f, 0.0f, 0.0f});
    }

    Draw createDraw(const Mesh & mesh)
    {
        auto compilation = MeshCompiler().compile(mesh);

        auto draw = context().createDraw(m_program, gl::GL_TRIANGLES);
        draw.addVertices(compilation.vertices.layout(), compilation.vertices.rawData());
        draw.setIndices32(compilation.indices.rawData());

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

        m_unsmoothedCubeDraw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_unsmoothedCubeDraw.uniform("Transform").set(m_transform[0].matrix());
        m_smoothedCubeDraw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_smoothedCubeDraw.uniform("Transform").set(m_transform[1].matrix());

        m_clear.schedule();
        m_unsmoothedCubeDraw.schedule();
        m_smoothedCubeDraw.schedule();
        m_grid.get().draw();
    }

private:
    Draw                                m_unsmoothedCubeDraw;
    Draw                                m_smoothedCubeDraw;
    Program                             m_program;
    Clear                               m_clear;
    Optional<DebugGrid3DRenderer>       m_grid;
    Camera3D                            m_camera;
    Transform3D                         m_transform[2];
    Optional<DebugCameraNavigator3D>    m_navigator;

};

int main(int argc, char * argv[])
{
    return EdgeToFaceSmoothingExample().run();
}

