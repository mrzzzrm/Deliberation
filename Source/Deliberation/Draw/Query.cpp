#include <Deliberation/Draw/Query.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/QueryImpl.h"

namespace deliberation
{
int64_t Query::resultI() const
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->resultI();
}

uint64_t Query::resultU() const
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->resultU();
}

QueryType Query::type() const
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->type();
}

bool Query::isActive() const
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->isActive();
}

bool Query::isResultAvailable() const
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->isResultAvailable();
}

void Query::begin()
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->begin();
}

void Query::end()
{
    AssertM(m_impl.get(), "Query is hollow");

    return m_impl->end();
}

Query::Query(const std::shared_ptr<QueryImpl> & impl) : Base(impl) {}
}
