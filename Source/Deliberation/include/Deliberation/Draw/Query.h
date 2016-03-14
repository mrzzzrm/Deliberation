#pragma once

#include <stdint.h>
#include <memory>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Enum.h>

namespace deliberation
{

namespace detail
{
    class QueryImpl;
}

class DELIBERATION_API Query final
{
public:
    Query();
    ~Query();

    int64_t resultI() const;
    uint64_t resultU() const;

    QueryType type() const;

    bool isActive() const;
    bool isResultAvailable() const;

    void begin();
    void end();

private:
    friend class Context;

private:
    Query(const std::shared_ptr<detail::QueryImpl> & impl);

private:
    std::shared_ptr<detail::QueryImpl> m_impl;
};

}

