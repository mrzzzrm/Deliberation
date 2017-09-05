#include <Deliberation/Physics/PrimitiveTester.h>

#include <Deliberation/Physics/RigidBody.h>

#define VERBOSE 0

namespace deliberation
{
void PrimitiveTester::lineTest(
    const Ray3D &                                            ray,
    const std::shared_ptr<RigidBody> & body,
    const std::function<bool(const RayCastIntersection &)> & handler) const
{
    auto it = m_primitiveTestByShapeType.find(body->shape()->type());

    if (it == m_primitiveTestByShapeType.end())
    {
        return;
    }

    auto & primitiveTest = it->second;
    auto   intersection = primitiveTest->lineTest(ray, body);

    if (intersection)
    {
        handler(*intersection);
    }
}

void PrimitiveTester::registerPrimitiveTest(
    int shapeType, std::unique_ptr<PrimitiveTestAlgorithm> && primitiveTest)
{
    AssertM(
        m_primitiveTestByShapeType.count(shapeType) == 0,
        "Shape type already registered");
    m_primitiveTestByShapeType[shapeType] = std::move(primitiveTest);
}
}