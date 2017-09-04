#pragma once

#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btMatrix3x3.h>
#include <Bullet3Common/b3Vector3.h>
#include <Bullet3Common/b3Transform.h>
#include <Bullet3Common/b3Quaternion.h>
#include <Bullet3Common/b3Matrix3x3.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{

inline glm::vec3 BulletPhysicsConvert(const btVector3 & bv)
{
    return glm::vec3(bv.x(), bv.y(), bv.z());
}

inline btVector3 BulletPhysicsConvert(const glm::vec3 & bv)
{
    return btVector3(bv.x, bv.y, bv.z);
}

inline glm::quat BulletPhysicsConvert(const btQuaternion & q)
{
    return glm::quat(q.w(), q.x(), q.y(), q.z());
}

inline btQuaternion BulletPhysicsConvert(const glm::quat & q)
{
    return {q.x, q.y, q.z, q.w};
}

//inline glm::mat3 BulletPhysicsConvert(const btMatrix3x3 & bulletMatrix)
//{
//    return glm::mat3(
//        BulletPhysicsConvert(bulletMatrix.getColumn(0)),
//        BulletPhysicsConvert(bulletMatrix.getColumn(1)),
//        BulletPhysicsConvert(bulletMatrix.getColumn(2))
//    );
//}

inline btTransform BulletPhysicsConvert(const Transform3D & transform)
{
    return btTransform(BulletPhysicsConvert(transform.orientation()),
                       BulletPhysicsConvert(transform.position()));
}

inline Transform3D BulletPhysicsConvert(const btTransform & bulletTransform)
{
    Transform3D result;
    result.setPosition(BulletPhysicsConvert(bulletTransform.getOrigin()));
    result.setOrientation(BulletPhysicsConvert(bulletTransform.getRotation()));

    return result;
}

}