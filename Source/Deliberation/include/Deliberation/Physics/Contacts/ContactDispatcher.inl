#pragma once

#include <Deliberation/Physics/Contacts/ContactDispatcher.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Physics/Contacts/Contact.h>
#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

template<typename T>
void ContactDispatcher::registerContactType(int typeA, int typeB)
{
    Assert(typeA >= 0 && typeB >= 0, "");

    class Creator:
        public ContactCreator
    {
    public:
        Creator(int typeL, int typeR):
            ContactCreator(typeL, typeR)
        {
        }

        virtual std::unique_ptr<Contact> create(RigidBody & bodyA, RigidBody & bodyB) const override
        {
            if (bodyA.shape()->type() != m_typeL)
            {
                Assert(bodyA.shape()->type() == m_typeR, "");
                Assert(bodyB.shape()->type() == m_typeL, "");

                return std::unique_ptr<Contact>(new T(bodyB, bodyA));
            }
            else
            {
                Assert(bodyA.shape()->type() == m_typeL, "");
                Assert(bodyB.shape()->type() == m_typeR, "");

                return std::unique_ptr<Contact>(new T(bodyA, bodyB));
            }
        }
    };

    m_doubleDispatch.insert(typeA, typeB, std::unique_ptr<ContactCreator>(new Creator(typeA, typeB)));
}

}