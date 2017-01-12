#pragma once

#include <memory>
#include <unordered_map>

#include <Deliberation/Core/Dispatch.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Physics/Contacts/ContactDispatcher.h>
#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Contact;

class DELIBERATION_API Narrowphase final
{
public:
    Narrowphase();
    ~Narrowphase();

    ContactDispatcher & contactDispatcher();
    const ContactDispatcher & contactDispatcher() const;
    const SparseVector<std::unique_ptr<Contact>> & contacts() const;

    bool hasContact(void * userDataA, void * userDataB) const;
    void addContact(void * userDataA, void * userDataB);
    void removeContact(void * userDataA, void * userDataB);

    void lineTest(const Ray3D &ray,
                  const std::shared_ptr<BroadphaseProxy> &proxy,
                  const std::function<bool(const RayCastIntersection &)> &handler);

    void registerPrimitiveTest(int shapeType, std::unique_ptr<NarrowphasePrimitiveTest> && primitiveTest);

    void updateContacts();

private:
    ContactDispatcher                       m_contactDispatcher;
    SparseVector<std::unique_ptr<Contact>>  m_contacts;
    DoubleDispatch<size_t>                  m_keysToContactIndex;
    std::unordered_map<int, std::unique_ptr<NarrowphasePrimitiveTest>>
                                            m_primitiveTestByShapeType;
};

}