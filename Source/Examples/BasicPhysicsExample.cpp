#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/SphereShape.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

using namespace deliberation;

int main()
{
    PhysicsWorld world;

    auto bigSphereShape = std::make_shared<SphereCollisionShape>(5.0f);
    auto smallSphereShape = std::make_shared<SphereCollisionShape>(1.0f);

    auto bigSphere = std::make_shared<RigidBody>(bigSphereShape);
    bigSphere->transform().setPosition({0, -bigSphereShape->radius(), 0});
    bigSphere->setMass(3.0f);

    auto smallSphere = std::make_shared<RigidBody>(smallSphereShape);
    smallSphere->transform().setPosition({0, 10, 0});

    smallSphere->setLinearVelocity({0, -8, 0});

    world.addRigidBody(bigSphere);
    world.addRigidBody(smallSphere);

    for (auto i = 0; i < 2 * 60; i++) {
        world.update(0.016f);
        std::cout << i << ": SmallBox @ " << smallSphere->transform().position() << "; BigBox @ " << bigSphere->transform().position() << std::endl;
    }

    return 0;
}