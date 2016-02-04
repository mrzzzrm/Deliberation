#include <Deliberation/Draw/Program.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/ProgramImpl.h"

namespace deliberation
{

Program::Program() = default;

Context & Program::context() const
{
    Assert(m_impl.get(), "Program is hollow");

    return m_impl->context;
}

const ProgramInterface & Program::interface() const
{
    Assert(m_impl.get(), "Program is hollow");

    return m_impl->interface;
}

Program::Program(const std::shared_ptr<detail::ProgramImpl> & impl):
    m_impl(impl)
{
}

}


