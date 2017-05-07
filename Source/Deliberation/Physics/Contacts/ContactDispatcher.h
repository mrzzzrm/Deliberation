#pragma once

#include <memory>

#include <Deliberation/Core/Dispatch.h>

namespace deliberation
{
class Contact;
class RigidBody;

class ContactCreator
{
  public:
    ContactCreator(int typeL, int typeR);
    virtual ~ContactCreator();

    virtual std::unique_ptr<Contact>
    create(RigidBody & bodyA, RigidBody & bodyB) const = 0;

  protected:
    int m_typeL;
    int m_typeR;
};

class ContactDispatcher final
{
  public:
    ContactDispatcher();

    template<typename T>
    void registerContactType(int typeA);

    template<typename T>
    void registerContactType(int typeA, int typeB);

    std::unique_ptr<Contact>
    createContact(RigidBody & bodyA, RigidBody & bodyB) const;

  private:
    DoubleDispatch<std::unique_ptr<ContactCreator>> m_doubleDispatch;
};
}

#include <Deliberation/Physics/Contacts/ContactDispatcher.inl>