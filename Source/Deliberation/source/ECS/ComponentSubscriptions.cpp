#include <Deliberation/ECS/ComponentSubscriptions.h>

namespace deliberation
{

std::unordered_map<TypeID::value_t, std::vector<TypeID::value_t>>
    ComponentSubscriptionsBase::subscriptionsByComponentType;

}