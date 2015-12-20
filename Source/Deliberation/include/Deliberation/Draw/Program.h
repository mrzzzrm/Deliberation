#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class DrawImpl;
    class DrawExecution;
    class ProgramImpl;
}

class ProgramInterface;

class DELIBERATION_API Program
{
public:
    Program();

    const ProgramInterface & interface() const;

private:
    friend class Context;
    friend class Draw;
    friend class detail::DrawImpl;
    friend class detail::DrawExecution;

private:
    Program(const std::shared_ptr<detail::ProgramImpl> & impl);

private:
    std::shared_ptr<detail::ProgramImpl> m_impl;
};

}

