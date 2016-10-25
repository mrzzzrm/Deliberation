#pragma once

#include <memory>

#include <Deliberation/Core/Dispatch.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Contact;
class RigidBody;

class DELIBERATION_API ContactCreator
{
public:
    ContactCreator(int typeL, int typeR);
    virtual ~ContactCreator();

    virtual std::unique_ptr<Contact> create(RigidBody & bodyA, RigidBody & bodyB) const = 0;

protected:
    int m_typeL;
    int m_typeR;
};

class DELIBERATION_API ContactDispatcher final
{
public:
    ContactDispatcher();

    template<typename T>
    void registerContactType(int typeA, int typeB);

    std::unique_ptr<Contact> createContact(RigidBody & bodyA, RigidBody & bodyB) const;

private:
    DoubleDispatch<std::unique_ptr<ContactCreator>> m_doubleDispatch;
};

}

#include <Deliberation/Physics/Contacts/ContactDispatcher.inl>