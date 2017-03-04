#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentWrapper.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

using RigidBodyComponent = ComponentWrapper<std::shared_ptr<RigidBody>>;

}