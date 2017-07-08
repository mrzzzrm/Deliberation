#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{
Pose3D Pose3D::fromTransform(const Transform3D & transform)
{
    Pose3D pose;
    pose.setOrientation(transform.orientation());
    pose.setPosition(transform.position());
    pose.setCenter(transform.center());
    return pose;
}
}