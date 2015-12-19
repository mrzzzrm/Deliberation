#include <Deliberation/Draw/Program.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/ProgramImpl.h"

namespace deliberation
{

Program::Program() = default;

const ProgramInterface & Program::interface() const
{
    Assert(m_impl.get(), "Can't perform action on hollow object");

    return m_impl->interface;
}

Program::Program(const std::shared_ptr<detail::ProgramImpl> & impl):
    m_impl(impl)
{
}

}


