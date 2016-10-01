#include <iostream>
#include <stdlib.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/DebugGeometryRenderer.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>

using namespace deliberation;

class DebugGeometryRendererExample:
    public Application
{
public:
    DebugGeometryRendererExample():
        Application("DebugGeometryRendererExample", "Deliberation2")
    {

    }

    virtual void onStartup() override
    {
        std::cout << "------------------------ DebugGeometryRendererExample ------------------------" << std::endl;

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)Application::context().backbuffer().width()/Application::context().backbuffer().height());

        m_grid.reset(context(), 1.0f, m_camera);
        m_navigator.reset(m_camera, inputAdapter(), 10.0f);
        m_debug.reset(context(), m_camera);

        m_clear = context().createClear();

        m_debug.get().addArrow({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f});
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        m_clear.schedule();
        m_grid.get().draw();
        m_debug.get().schedule();
    }

private:
    Optional<DebugGrid3DRenderer>    m_grid;
    Optional<DebugCameraNavigator3D> m_navigator;
    Camera3D                         m_camera;
    Clear                            m_clear;
    Optional<DebugGeometryRenderer>  m_debug;
};

int main()
{
    DebugGeometryRendererExample().run();

    return 0;
}