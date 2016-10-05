#pragma once

#include <Deliberation/Physics/Contacts/Contact.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class CollisionObject;
class SphereShape;

class DELIBERATION_API SphereContact:
    public Contact
{
public:
    SphereContact(RigidBody & bodyA, RigidBody & objectB);

    virtual void update() override;

private:
    const SphereShape & m_shapeA;
    const SphereShape & m_shapeB;
};

}