#include <Deliberation/Physics/Contacts/ContactDispatcher.h>

#include <algorithm>
#include <string>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Physics/Contacts/BoxContact.h>
#include <Deliberation/Physics/Contacts/SphereContact.h>
#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{
ContactCreator::ContactCreator(int typeL, int typeR)
    : m_typeL(typeL), m_typeR(typeR)
{
}

ContactCreator::~ContactCreator() = default;

ContactDispatcher::ContactDispatcher()
{
    registerContactType<SphereContact>(
        (int)CollisionShape_Sphere, (int)CollisionShape_Sphere);
    registerContactType<BoxContact>(
        (int)CollisionShape_Box, (int)CollisionShape_Box);
}

std::unique_ptr<Contact>
ContactDispatcher::createContact(RigidBody & bodyA, RigidBody & bodyB) const
{
    auto typeA = bodyA.shape()->type();
    auto typeB = bodyB.shape()->type();

    AssertM(
        m_doubleDispatch.contains(typeA, typeB),
        "No Algorithm registered for CollisionShapeTypes " +
            std::to_string(typeA) + "/" + std::to_string(typeB));

    auto & creator = m_doubleDispatch.dispatch(typeA, typeB);

    return creator->create(bodyA, bodyB);
}
}