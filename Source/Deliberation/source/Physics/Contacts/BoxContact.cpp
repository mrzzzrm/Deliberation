#include <Deliberation/Physics/Contacts/BoxContact.h>

namespace deliberation
{

    BoxContact(RigidBody & bodyA, RigidBody & bodyB);

    void BoxContact::update() override;

}
