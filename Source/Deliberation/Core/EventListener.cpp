#include <Deliberation/Core/EventListener.h>

#include <Deliberation/Core/EventDomain.h>

namespace deliberation
{

EventListener::~EventListener()
{
    auto domain = m_domain.lock();
    if (domain) domain->removeEventListener(*this);
}

}
