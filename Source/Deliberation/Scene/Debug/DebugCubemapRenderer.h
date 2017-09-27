#pragma once

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Scene/Camera3D.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DebugCubemapRenderer final
{
public:
    enum class MeshType
    {
        Cube,
        Sphere
    };

public:
    DebugCubemapRenderer(
        const Camera3D & camera,
        const Texture &  cubemap,
        MeshType         meshType = MeshType::Cube);

    const Pose3D & pose() const;

    void setPose(const Pose3D & pose);

    void render();

private:
    const Camera3D & m_camera;

    Pose3D m_pose;

    Draw    m_draw;
    Uniform m_transformUniform;
    Uniform m_viewProjectionUniform;
};
}