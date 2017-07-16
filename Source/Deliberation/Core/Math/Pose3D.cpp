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

Pose3D Pose3D::fromTransformedPose(const Transform3D & transform, const Pose3D & pose)
{
    Pose3D result;
    result.setOrientation(transform.orientation() * pose.orientation());
    result.setPosition(transform.pointLocalToWorld(pose.position()));
    result.setCenter(pose.center());
    return result;
}
}