#include <Deliberation/Physics/Narrowphase.h>

#include <iostream>

#include <Deliberation/Physics/BroadphaseProxy.h>
#include <Deliberation/Physics/Contacts/Contact.h>
#include <Deliberation/Physics/CollisionObject.h>
#include <Deliberation/Physics/RigidBody.h>

#define VERBOSE 0

namespace deliberation
{

Narrowphase::Narrowphase() = default;

Narrowphase::~Narrowphase() = default;

ContactDispatcher & Narrowphase::contactDispatcher()
{
    return m_contactDispatcher;
}

const ContactDispatcher & Narrowphase::contactDispatcher() const
{
    return m_contactDispatcher;
}

const SparseVector<std::unique_ptr<Contact>> & Narrowphase::contacts() const
{
    return m_contacts;
}

bool Narrowphase::hasContact(void * userDataA, void * userDataB) const
{
    auto & bodyA = *(RigidBody*)userDataA;
    auto & bodyB = *(RigidBody*)userDataB;

    auto indexA = bodyA.index();
    auto indexB = bodyB.index();

    return m_keysToContactIndex.contains(indexA, indexB);
}

void Narrowphase::addContact(void * userDataA, void * userDataB)
{
    auto & bodyA = *(RigidBody*)userDataA;
    auto & bodyB = *(RigidBody*)userDataB;

#if VERBOSE
    std::cout << "Narrowphase::addContact(): " << userDataA << " and " << userDataB << std::endl;
#endif

    auto shapeTypeA = bodyA.shape()->type();
    auto shapeTypeB = bodyB.shape()->type();

    auto contact = m_contactDispatcher.createContact(bodyA, bodyB);

    auto index = m_contacts.insert(std::move(contact));

    auto indexA = bodyA.index();
    auto indexB = bodyB.index();

    m_keysToContactIndex.insert(indexA, indexB, index);
}

void Narrowphase::removeContact(void * userDataA, void * userDataB)
{
    auto & bodyA = *(RigidBody*)userDataA;
    auto & bodyB = *(RigidBody*)userDataB;

    auto indexA = bodyA.index();
    auto indexB = bodyB.index();

    auto index = m_keysToContactIndex.dispatch(indexA, indexB);
    m_keysToContactIndex.erase(indexA, indexB);

    m_contacts.erase(index);
}

void Narrowphase::lineTest(const Ray3D &ray,
                           const std::shared_ptr<BroadphaseProxy> &proxy,
                           const std::function<bool(const RayCastIntersection &)> &handler)
{
    auto & body = *(RigidBody*)proxy->userData();

    auto it = m_primitiveTestByShapeType.find(body.shape()->type());

    if (it == m_primitiveTestByShapeType.end())
    {
        return;
    }

    auto & primitiveTest = it->second;
    auto intersection = primitiveTest->lineTest(ray, body);

    if (intersection)
    {
        handler(*intersection);
    }
}

void Narrowphase::registerPrimitiveTest(int shapeType, std::unique_ptr<NarrowphasePrimitiveTest> && primitiveTest)
{
    Assert(m_primitiveTestByShapeType.count(shapeType) == 0, "Shape type already registered");
    m_primitiveTestByShapeType[shapeType] = std::move(primitiveTest);
}

void Narrowphase::updateContacts()
{
    for (auto & contact : m_contacts)
    {
        contact->update();
    }
}

}