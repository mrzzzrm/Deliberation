#include <Deliberation/Core/EventListenerProxy.h>

#include <Deliberation/Core/EventDomain.h>

namespace deliberation
{

EventListenerProxy::~EventListenerProxy()
{
    auto domain = m_domain.lock();
    if (domain) domain->removeEventListener(*this);
}

}
