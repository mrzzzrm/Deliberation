#pragma once

#include <Deliberation/Physics/Contacts/Contact.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API BoxContact final:
    public Contact
{
public:
    BoxContact(RigidBody & bodyA, RigidBody & bodyB);

    virtual void update() override;
};

}