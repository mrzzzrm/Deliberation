#include <iostream>

#include <btBulletDynamicsCommon.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

using namespace deliberation;

struct Foo
{
    int i = 42;
    double d = 4.2f;
};

int main()
{
    SparseVector<Foo> foos;

    foos.insert(Foo());
    foos.insert(Foo());

    foos.erase(0);
    foos.erase(1);

    return 0;
}
