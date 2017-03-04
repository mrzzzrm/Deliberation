#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{

template<typename ... T>
void DELIBERATION_API Swallow(T &&...) {}

}