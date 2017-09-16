#include <Deliberation/ECS/Phase.h>

#include <Deliberation/ECS/ActivityManager.h>

namespace deliberation
{

TypeID::value_t GameUpdatePhase::phaseTypeId()
{
    return TypeID::value<ActivityManager, GameUpdatePhase>();
}

}