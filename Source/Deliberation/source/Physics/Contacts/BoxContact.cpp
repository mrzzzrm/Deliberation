#include <Deliberation/Physics/Contacts/BoxContact.h>

#include <iostream>
#include <limits>
#include <string.h>
#include <stdlib.h>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/PolygonClipping.h>

#include <Deliberation/Physics/Contacts/Manifold.h>
#include <Deliberation/Physics/BoxShape.h>
#include <Deliberation/Physics/RigidBody.h>

namespace
{

using namespace deliberation;

struct Face
{
    u8 normalIndex;
    u8 xAxisIndex;
    u8 yAxisIndex;
    glm::vec3 normalAxis;
    glm::vec3 xAxis;
    glm::vec3 yAxis;
    glm::vec3 center;
};

struct BoxContactAlgorithm
{

const Box & boxA;
const Box & boxB;
Manifold & manifold;
CollideBoxBoxDebugInfo * debugInfo;

glm::mat3 aInvOrientation;
glm::mat3 bInvOrientation;

glm::vec3 aPositionDelta;
glm::vec3 worldPositionDelta;
glm::vec3 aNormal;
glm::vec3 bNormal;

void rayClosestApproach(const glm::vec3 & oA, const glm::vec3 & dA, const glm::vec3 & oB, const glm::vec3 & dB, float & a, float & b)
{
    glm::vec3 p = oB - oA;
    auto uaub = glm::dot(dA, dB);
    auto q1 =  glm::dot(dA, p);
    auto q2 = -glm::dot(dB, p);
    auto d = 1-uaub*uaub;
    if (d <= 0.0001f) {
        a = 0;
        b  = 0;
    }
    else {
        d = 1.0f / d;
        a = (q1 + uaub*q2)*d;
        b  = (uaub*q1 + q2)*d;
    }

}

glm::vec3 normalFromDirection(const glm::vec3 & direction)
{
    auto dot = glm::dot(direction, worldPositionDelta);

    if (dot < 0)
    {
        return glm::normalize(-direction);
    }
    else
    {
        return glm::normalize(direction);
    }
}

Face computeFaceFromNormal(const Box & box, const glm::vec3 & normal)
{
    Face face;

    auto normalAbs = glm::abs(aNormal);

    if (normalAbs.x >= normalAbs.y)
    {
        if (normalAbs.x >= normalAbs.z)
        {
            face.normalIndex = 0;
            face.xAxisIndex = 1;
            face.yAxisIndex = 2;
        }
        else
        {
            face.normalIndex = 2;
            face.xAxisIndex = 0;
            face.yAxisIndex = 1;
        }
    }
    else
    {
        if (normalAbs.y >= normalAbs.z)
        {
            face.normalIndex = 1;
            face.xAxisIndex = 0;
            face.yAxisIndex = 2;
        }
        else
        {
            face.normalIndex = 2;
            face.xAxisIndex = 0;
            face.yAxisIndex = 1;
        }
    }

    face.normalAxis = (normal[face.normalIndex] > 0 ? 1.0f : -1.0f) * box.axis(face.normalIndex);
    face.xAxis = box.axis(face.xAxisIndex);
    face.yAxis = box.axis(face.yAxisIndex);

    face.center = box.p() + face.normalAxis * box.halfExtent()[face.normalIndex];

    return face;
}

BoxContactAlgorithm(const Box & boxA, const Box & boxB, Manifold & manifold, CollideBoxBoxDebugInfo * debugInfo):
    boxA(boxA),
    boxB(boxB),
    manifold(manifold),
    debugInfo(debugInfo)
{

}

bool CollideFaceSomething(const glm::vec3 & direction,
                          const Box & boxA,
                          const Box & boxB,
                          Manifold & manifold)
{
    auto checkDirection = glm::normalize(direction);

    auto minA = std::numeric_limits<float>::max();
    auto maxA = std::numeric_limits<float>::lowest();
    auto minB = std::numeric_limits<float>::max();
    auto maxB = std::numeric_limits<float>::lowest();

    for (auto i = 0; i < 8; i++)
    {
        auto r = glm::dot(boxA.point(i), checkDirection);
        minA = std::min(r, minA);
        maxA = std::max(r, maxA);
    }

    for (auto i = 0; i < 8; i++)
    {
        auto r = glm::dot(boxB.point(i), checkDirection);
        minB = std::min(r, minB);
        maxB = std::max(r, maxB);
    }

    auto diffA = maxA - minA;
    auto diffB = maxB - minB;
    auto diffCombined = std::max(maxA, maxB) - std::min(minA, minB);

    auto depth = diffA + diffB - diffCombined;

    if (depth > 0)
    {
        if (depth < manifold.depth())
        {

            if (debugInfo)
            {
                debugInfo->edgeCollision = false;
            }

            manifold.setDepth(depth);
            manifold.setNormal(normalFromDirection(checkDirection));

            aNormal = aInvOrientation * manifold.normal();
            bNormal = bInvOrientation * -manifold.normal();

            Face rFace = computeFaceFromNormal(boxA, aNormal);
            Face iFace = computeFaceFromNormal(boxB, bNormal);

            if (debugInfo)
            {
                debugInfo->incidentFaceCenter = iFace.center;
                debugInfo->incidentFaceNormal = iFace.normalAxis;
                debugInfo->referenceFaceCenter = rFace.center;
                debugInfo->referenceFaceNormal = rFace.normalAxis;
            }

            glm::mat3 rFaceRot = glm::mat3(rFace.xAxis, rFace.yAxis, rFace.normalAxis);
            glm::mat3 invRFaceRot = glm::transpose(rFaceRot);

            /**
             * Project incident face on reference face
             */
            auto x = iFace.xAxis * boxB.halfExtent()[iFace.xAxisIndex];
            auto y = iFace.yAxis * boxB.halfExtent()[iFace.yAxisIndex];

            glm::vec3 in3[4];
            in3[0] = iFace.center + x + y;
            in3[1] = iFace.center - x + y;
            in3[2] = iFace.center - x - y;
            in3[3] = iFace.center + x - y;

            if (debugInfo)
            {
                memcpy(debugInfo->incidentFace, in3, sizeof(in3));
            }

            glm::vec2 ref2[4];
            {
                auto x = boxA.halfExtent()[rFace.xAxisIndex];
                auto y = boxA.halfExtent()[rFace.yAxisIndex];

                ref2[0] = glm::vec2( x, y);
                ref2[1] = glm::vec2(-x, y);
                ref2[2] = glm::vec2(-x,-y);
                ref2[3] = glm::vec2( x,-y);
            }

            if (debugInfo)
            {
                for (uint v = 0; v < 4; v++)
                {
                    debugInfo->referenceFace[v] = rFaceRot * glm::vec3(ref2[v].x, ref2[v].y, 0) + rFace.center;
                }
            }

            /**
             * Clip projection
             */
            std::vector<glm::vec2> result;
            glm::vec2 subject[4];

            for (int i = 0; i < 4; i++)
            {
                in3[i] = invRFaceRot * (in3[i] - rFace.center);

                subject[i].x = in3[i].x;
                subject[i].y = in3[i].y;
            }

            PolygonClipping2D(subject, ref2, result);

            if (debugInfo)
            {
                debugInfo->numClipPoints = result.size();

                for (uint v = 0; v < debugInfo->numClipPoints; v++)
                {
                    debugInfo->clipPoints[v] = rFaceRot * glm::vec3(result[v].x, result[v].y, 0) + rFace.center;
                }
            }

            /**
             * Incident face center and normal in reference face coordinates
             */
            glm::vec3 inCenterRef = invRFaceRot * (iFace.center - rFace.center);
            glm::vec3 inNormalRef = invRFaceRot * iFace.normalAxis;

            /**
             * Pick the result point with the greatest depth
             */
            auto resultIndex = -1;
            {
                Assert(inNormalRef.z != 0.0f, "");

                auto maxZ = std::numeric_limits<float>::max();

                for (auto r = 0u; r < result.size(); r++)
                {
                    auto x = result[r].x;
                    auto y = result[r].y;
                    auto & n = inNormalRef;
                    auto nom =  - (x - inCenterRef.x) * n.x - (y - inCenterRef.y) * n.y;

                    auto z = (nom / (n.z)) + inCenterRef.z;
                    if (z < maxZ)
                    {
                        maxZ = z;
                        resultIndex = r;
                    }
                }
            }
            if (resultIndex == -1)
            {
                return true;
            }

            /**
             * Compute contact point
             */
            auto c = rFaceRot * glm::vec3(result[resultIndex].x, result[resultIndex].y, 0) + rFace.center;
            manifold.setPosition(c);
            manifold.setLocalPointA(glm::transpose(boxA.orientationMatrix()) * (c - boxA.p()));
            manifold.setLocalPointB(glm::transpose(boxB.orientationMatrix()) * (c - boxB.p()));
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool CollideEdgeEdge(const glm::vec3 & dirA, const glm::vec3 & dirB, const Box & boxA, const Box & boxB, Manifold & manifold)
{
    auto direction = glm::cross(dirA, dirB);

    auto checkDirection = glm::normalize(direction);

    auto minA = std::numeric_limits<float>::max();
    auto maxA = std::numeric_limits<float>::lowest();
    auto minB = std::numeric_limits<float>::max();
    auto maxB = std::numeric_limits<float>::lowest();

    for (auto i = 0; i < 8; i++)
    {
        auto r = glm::dot(boxA.point(i), checkDirection);
        minA = std::min(r, minA);
        maxA = std::max(r, maxA);
    }

    for (auto i = 0; i < 8; i++)
    {
        auto r = glm::dot(boxB.point(i), checkDirection);
        minB = std::min(r, minB);
        maxB = std::max(r, maxB);
    }

    auto diffA = maxA - minA;
    auto diffB = maxB - minB;
    auto diffCombined = std::max(maxA, maxB) - std::min(minA, minB);

    auto depth = diffA + diffB - diffCombined;

    if (depth > 0)
    {
        if (depth < manifold.depth())
        {
            if (debugInfo)
            {
                debugInfo->edgeCollision = true;
            }

            manifold.setDepth(depth);
            manifold.setNormal(normalFromDirection(checkDirection));

            aNormal = aInvOrientation * manifold.normal();
            bNormal = bInvOrientation * -manifold.normal();

            glm::vec3 pointA = boxA.p();
            for (auto i = 0; i < 3; i++)
            {
                pointA += (aNormal[i] > 0 ? 1.0f : -1.0f) * boxA.halfExtent()[i] * boxA.axis(i);
            }
            glm::vec3 pointB = boxB.p();
            for (auto i = 0; i < 3; i++)
            {
                pointB += (bNormal[i] > 0 ? 1.0f : -1.0f) * boxB.halfExtent()[i] * boxB.axis(i);
            }

            auto a = 0.0f;
            auto b = 0.0f;
            rayClosestApproach(pointA, dirA, pointB, dirB, a, b);

            auto contactA = pointA + a * dirA;
            auto contactB = pointB + b * dirB;

            auto contact = (contactA + contactB) * 0.5f;

            manifold.setPosition(contact);
            manifold.setLocalPointA(glm::transpose(boxA.orientationMatrix()) * (contact - boxA.p()));
            manifold.setLocalPointB(glm::transpose(boxB.orientationMatrix()) * (contact - boxB.p()));
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool CollideEdgesEdges(const Box & boxA, const Box & boxB, Manifold & manifold)
{
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            auto a = boxA.axis(i);
            auto b = boxB.axis(j);

            auto direction = glm::cross(a, b);
            if (direction != glm::vec3(0.0f))
            {
                if (!CollideEdgeEdge(a, b, boxA, boxB, manifold))
                {
                    std::cout << "EENo collision in direction " << direction << std::endl;
                    return false;
                }
            }
        }
    }

    return true;
}

bool execute()
{
    manifold.setDepth(std::numeric_limits<float>::max());

    worldPositionDelta = boxB.p() - boxA.p();
    aInvOrientation = glm::transpose(boxA.orientationMatrix());
    bInvOrientation = glm::transpose(boxB.orientationMatrix());
    aPositionDelta = aInvOrientation * worldPositionDelta;


    if (!CollideFaceSomething(boxA.x(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.x() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxA.y(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.y() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxA.z(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.z() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxB.x(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.x() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxB.y(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.y() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxB.z(), boxA, boxB, manifold))
    {
        //std::cout << "No collision in direction " << boxA.z() << std::endl;
        return false;
    }

    if (!CollideEdgesEdges(boxA, boxB, manifold))
    {
        return false;
    }

    return true;
}

};

}

namespace deliberation
{

bool DELIBERATION_API CollideBoxBox(const Box & boxA,
                                    const Box & boxB,
                                    Manifold & manifold,
                                    CollideBoxBoxDebugInfo * debugInfo)
{
    BoxContactAlgorithm algorithm(boxA, boxB, manifold, debugInfo);
    return algorithm.execute();
}

BoxContact::BoxContact(RigidBody & bodyA, RigidBody & bodyB):
    Contact(bodyA, bodyB)
{

}

void BoxContact::update()
{
    /**
     * Retrieve boxes
     */
    auto & shapeA = (BoxShape&)*m_bodyA.shape();
    auto & shapeB = (BoxShape&)*m_bodyB.shape();
    auto boxA = shapeA.instanciate(m_bodyA.transform());
    auto boxB = shapeB.instanciate(m_bodyB.transform());

    /**
     * Intersect
     */
    CollideBoxBoxDebugInfo info;
    Manifold manifold;
    m_intersect = CollideBoxBox(boxA, boxB, manifold, &info);

    if (m_intersect)
    {
        m_position = manifold.position();
        m_localPointA = manifold.localPositionA();
        m_localPointB = manifold.localPositionB();
        m_normal = manifold.normal();
        m_separation = manifold.depth();

        auto & vA = m_bodyA.linearVelocity();
        auto & vB = m_bodyB.linearVelocity();

        auto mA = m_bodyA.inverseMass();
        auto mB = m_bodyB.inverseMass();

        auto & iA = m_bodyA.worldInverseInertia();
        auto & iB = m_bodyB.worldInverseInertia();

        auto rA = m_position - m_bodyA.transform().position();
        auto rB = m_position - m_bodyB.transform().position();

        auto rnA = glm::cross(rA, m_normal);
        auto rnB = glm::cross(rB, m_normal);

        auto inertiaA = glm::dot(m_normal, glm::cross(iA * rnA, rA));
        auto inertiaB = glm::dot(m_normal, glm::cross(iB * rnB, rB));

        auto kNormal = mA + mB + inertiaA + inertiaB;

        m_normalMass = kNormal > 0.0f ? 1.0f / kNormal : 0.0f;

        //   b2Vec2 tangent = b2Cross(vc->normal, 1.0f);

        // float32 rtA = b2Cross(vcp->rA, tangent);
        // float32 rtB = b2Cross(vcp->rB, tangent);

        //   float32 kTangent = mA + mB + iA * rtA * rtA + iB * rtB * rtB;

        //vcp->tangentMass = kTangent > 0.0f ? 1.0f /  kTangent : 0.0f;

        // Setup a velocity bias for restitution.
        m_velocityBias = 0.0f;
        auto vRel = glm::dot(m_normal, vB - vA /*- b2Cross(wA, vcp->rA)+ glm::cross(wB, vcp->rB) */);
        if (vRel < -1.0f)
        {
            auto restitution = std::max(m_bodyA.restitution(), m_bodyB.restitution());
            m_velocityBias = -restitution * vRel;
        }
    }

}

}
