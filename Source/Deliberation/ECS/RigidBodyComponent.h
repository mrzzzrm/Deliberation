#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentWrapper.h>

#include <Deliberation/ImGui/ImGui.h>

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


    virtual void renderImGui() {
        auto angularVelocity = value()->bulletRigidBody()->getAngularVelocity();
        auto linearVelocity = value()->bulletRigidBody()->getLinearVelocity();

        ImGui::InputFloat3("Angular Velocity", angularVelocity.operator btScalar*());
        ImGui::InputFloat3("Linear Velocity", linearVelocity.operator btScalar*());

        ImGuiProperty(value()->transform());
    }
};

}