#include <Deliberation/Physics/PrimitiveTestAlgorithm.h>

namespace deliberation
{
RayCastIntersection::RayCastIntersection(std::shared_ptr<RigidBody> body)
    : body(body)
{
}
}