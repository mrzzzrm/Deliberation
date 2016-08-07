#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

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

    void setTransform(const Transform3D & transform);

private:
    std::shared_ptr<CollisionShape> m_shape;
    Transform3D                     m_transform;
};

}