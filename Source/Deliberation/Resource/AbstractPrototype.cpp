#include "AbstractPrototype.h"

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

PrototypeManager & AbstractPrototype::prototypeManagerRef() const
{
    auto prototypeManager = m_prototypeManager.lock();
    Assert(static_cast<bool>(prototypeManager), "");
    return *prototypeManager;
}

}