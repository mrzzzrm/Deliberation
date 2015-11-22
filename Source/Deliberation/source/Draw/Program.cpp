#include <Deliberation/Draw/Program.h>

#include "Detail/ProgramImpl.h"

namespace deliberation
{

const ProgramInterface & Program::interface() const
{
    return m_impl->interface;
}

}

