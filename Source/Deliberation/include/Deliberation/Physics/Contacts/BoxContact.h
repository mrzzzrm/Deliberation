#pragma once

#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Physics/Contacts/Contact.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Manifold;

bool DELIBERATION_API CollideBoxBox(const Box & boxA, const Box & boxB, Manifold & manifold);

class DELIBERATION_API BoxContact final:
    public Contact
{
public:
    BoxContact(RigidBody & bodyA, RigidBody & bodyB);

    virtual void update() override;
};

}