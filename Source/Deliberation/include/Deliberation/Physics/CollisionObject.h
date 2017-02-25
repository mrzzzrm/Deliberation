#pragma once

#include <memory>

#include <Deliberation/Core/Math/AABB.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class BroadphaseProxy;
class CollisionShape;

class DELIBERATION_API CollisionObject
{
public:
    CollisionObject(const std::shared_ptr<CollisionShape> & shape,
                    const Transform3D & transform = Transform3D());
    virtual ~CollisionObject();

    const std::shared_ptr<CollisionShape> & shape() const;
    Transform3D & transform();
    const Transform3D & transform() const;
    AABB bounds() const;
    const std::shared_ptr<BroadphaseProxy> & proxy() const;

    void setShape(const std::shared_ptr<CollisionShape> & shape);
    void setTransform(const Transform3D & transform);
    void setProxy(const std::shared_ptr<BroadphaseProxy> & proxy);

protected:
    std::shared_ptr<CollisionShape>     m_shape;
    Transform3D                         m_transform;
    std::shared_ptr<BroadphaseProxy>    m_proxy;
};

}