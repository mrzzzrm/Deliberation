#include <Deliberation/Draw/Program.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/ProgramImpl.h"

namespace deliberation
{

Program::Program() = default;

DrawContext & Program::drawContext() const
{
    Assert(m_impl.get(), "Program is hollow");

    return m_impl->drawContext;
}

const ProgramInterface & Program::interface() const
{
    Assert(m_impl.get(), "Program is hollow");

    return m_impl->interface;
}

Program::Program(const std::shared_ptr<ProgramImpl> & impl):
    m_impl(impl)
{
}

}


