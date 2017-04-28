#include <Deliberation/Draw/Query.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/QueryImpl.h"

namespace deliberation
{

Query::Query() = default;

Query::~Query() = default;

int64_t Query::resultI() const
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->resultI();
}

uint64_t Query::resultU() const
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->resultU();
}

QueryType Query::type() const
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->type();
}

bool Query::isActive() const
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->isActive();
}

bool Query::isResultAvailable() const
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->isResultAvailable();
}

void Query::begin()
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->begin();
}

void Query::end()
{
    Assert(m_impl.get(), "Query is hollow");

    return m_impl->end();
}

Query::Query(const std::shared_ptr<detail::QueryImpl> & impl):
    m_impl(impl)
{

}

}

