#pragma once

#include <memory>

#include <Deliberation/Core/Dispatch.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Physics/Contacts/ContactDispatcher.h>
#include <Deliberation/Physics/Narrowphase.h>

#include <Deliberation/Deliberation_API.h>

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

    void updateContacts();

private:
    ContactDispatcher                       m_contactDispatcher;
    SparseVector<std::unique_ptr<Contact>>  m_contacts;
    DoubleDispatch<size_t>                  m_keysToContactIndex;
};

}