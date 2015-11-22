#pragma once

#include <memory>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class ProgramImpl;
}

class ProgramInterface;

class Program
{
public:
    const ProgramInterface & interface() const;

private:
    friend class Context;
    friend class detail::DrawExecution;

private:
    std::shared_ptr<detail::ProgramImpl> m_impl;
};

}

