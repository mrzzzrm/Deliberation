#pragma once

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>

#include <Deliberation/Platform/Application.h>

class SceneExampleApplication:
    public deliberation::Application
{
public:
    SceneExampleApplication(const std::string & name);

    virtual void onStartup() override;
    virtual void onFrame(float seconds) override;

protected:
    deliberation::Clear     m_clear;
    deliberation::Optional<deliberation::DebugGroundPlaneRenderer>
                            m_ground;
    deliberation::Camera3D  m_camera;
    deliberation::Optional<deliberation::DebugCameraNavigator3D>
                            m_navigator;
};