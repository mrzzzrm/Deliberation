#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentWrapper.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{
class RigidBodyComponent
    : public ComponentWrapper<std::shared_ptr<RigidBody>, RigidBodyComponent>
{
    DELIBERATION_COMPONENT_NAME("RigidBody")
public:
    RigidBodyComponent() = default;
    RigidBodyComponent(const std::shared_ptr<RigidBody> & rigidBody)
        : Base(rigidBody)
    {
    }
};

class Transform3DComponent
    : public ComponentWrapper<Transform3D, Transform3DComponent>
{
    DELIBERATION_COMPONENT_NAME("Transform3D")
public:
    void renderImGui() override;
};
}