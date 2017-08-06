#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Draw/GL/GLType.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>

using namespace deliberation;

class ImguiSandbox : public deliberation::Application
{
public:
    ImguiSandbox() : Application("ImguiSandbox") {}

    virtual void onStartup() override
    {
        //        EnableGLErrorChecksAndLogging();

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(
            (float)drawContext().backbuffer().width() /
            drawContext().backbuffer().height());

        //        m_ground =
        //        std::make_shared<DebugGroundPlaneRenderer>(drawContext(),
        //        m_camera); m_ground->setSize(30.0f);
        //        m_ground->setRadius(10.0f);

        m_world.addSystem<ApplicationSystem>(*this);
        m_world.addSystem<ImGuiSystem>();

        m_clear = drawContext().createClear();
    }

    virtual void onFrame(const UpdateFrame & updateFrame) override
    {
        m_clear.render();

        m_world.frameBegin();

        // m_ground->render();

        {
            bool open = false;
            ImGui::ShowTestWindow(&open);
        }

        m_world.render();
    }

private:
    Camera3D m_camera;
    Clear    m_clear;
    //    std::shared_ptr<DebugGroundPlaneRenderer>   m_ground;
    World m_world;
};

int main(int argc, char * argv[]) { return ImguiSandbox().run(argc, argv); }
