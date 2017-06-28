#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentWrapper.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{
using RigidBodyComponent = ComponentWrapper<std::shared_ptr<RigidBody>>;

class Transform3DComponent : public ComponentWrapper<Transform3D>
{
public:
    void renderImGui() override;
};

}