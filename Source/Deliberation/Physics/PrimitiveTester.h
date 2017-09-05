#pragma once

#include <memory>
#include <unordered_map>

#include <Deliberation/Core/Dispatch.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Physics/PrimitiveTestAlgorithm.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class Contact;

class PrimitiveTester final
{
public:
    void lineTest(
        const Ray3D &                                            ray,
        const std::shared_ptr<RigidBody> & body,
        const std::function<bool(const RayCastIntersection &)> & handler) const;

    void registerPrimitiveTest(
        int                                          shapeType,
        std::unique_ptr<PrimitiveTestAlgorithm> && primitiveTest);

private:
    std::unordered_map<int, std::unique_ptr<PrimitiveTestAlgorithm>>
        m_primitiveTestByShapeType;
};
}