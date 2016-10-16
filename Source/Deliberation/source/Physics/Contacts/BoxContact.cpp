#include <Deliberation/Physics/Contacts/BoxContact.h>

#include <iostream>
#include <limits>
#include <stdlib.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/PolygonClipping.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>

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


Face computeFaceFromNormal(const Box & box, const glm::vec3 & normal)
{
    Face face;

    auto normalAbs = glm::abs(normal);

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

bool compareIntersections(const ContactPoint & lhs, const ContactPoint & rhs)
{
    return lhs.depth > rhs.depth;
}

struct BoxContactAlgorithm
{
    uint & numContactPoints;
    ContactPoint * contactPoints;

    const Box & boxA;
    const Box & boxB;
    CollideBox3DDebugInfo * debugInfo;

    glm::mat3 aInvOrientation;
    glm::mat3 bInvOrientation;

    glm::vec3 worldPositionDelta;
    glm::vec3 aNormal;
    glm::vec3 bNormal;

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

    BoxContactAlgorithm(const Box & boxA,
                        const Box & boxB,
                        uint & numContactPoints,
                        ContactPoint * contactPoints,
                        CollideBox3DDebugInfo * debugInfo):
        boxA(boxA),
        boxB(boxB),
        numContactPoints(numContactPoints),
        contactPoints(contactPoints),
        debugInfo(debugInfo)
    {
        numContactPoints = 0;
    }

    bool CollideFaceSomething(const glm::vec3 & direction,
                              const Box & boxA,
                              const Box & boxB)
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
            if (debugInfo)
            {
                debugInfo->edgeCollision = false;
            }

            auto normal = normalFromDirection(checkDirection);

            aNormal = aInvOrientation * normal;
            bNormal = bInvOrientation * -normal;

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
             * Setup the intersection
             */
            Intersection intersection;
            intersection.position = rFaceRot * glm::vec3(result[resultIndex].x, result[resultIndex].y, 0) + rFace.center;
            intersection.normal = normal;
            intersection.depth = depth;

            intersectionFound(intersection);

            return true;
        }
        else
        {
            return false;
        }
    }

    bool CollideEdgeEdge(const glm::vec3 & dirA, const glm::vec3 & dirB, const Box & boxA, const Box & boxB)
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

        if (depth <= 0)
        {
            return false;
        }

        if (debugInfo)
        {
            debugInfo->edgeCollision = true;
        }

        auto normal = normalFromDirection(checkDirection);

        aNormal = aInvOrientation * normal;
        bNormal = bInvOrientation * -normal;

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
        Ray3DClosestApproach(pointA, dirA, pointB, dirB, a, b);

        auto contactA = pointA + a * dirA;
        auto contactB = pointB + b * dirB;

        auto position = (contactA + contactB) * 0.5f;

        /**
         * Add intersection
         */
        Intersection intersection;
        intersection.position = position;
        intersection.normal = normal;
        intersection.depth = depth;

        intersectionFound(intersection);

        return true;
    }

    bool CollideEdgesEdges(const Box & boxA, const Box & boxB)
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
                    if (!CollideEdgeEdge(a, b, boxA, boxB))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool execute()
    {
        worldPositionDelta = boxB.p() - boxA.p();
        aInvOrientation = glm::transpose(boxA.orientationMatrix());
        bInvOrientation = glm::transpose(boxB.orientationMatrix());

        if (!CollideFaceSomething(boxA.x(), boxA, boxB))
        {
            return false;
        }

        if (!CollideFaceSomething(boxA.y(), boxA, boxB))
        {
            return false;
        }

        if (!CollideFaceSomething(boxA.z(), boxA, boxB))
        {
            return false;
        }

        if (!CollideFaceSomething(boxB.x(), boxA, boxB))
        {
            return false;
        }

        if (!CollideFaceSomething(boxB.y(), boxA, boxB))
        {
            return false;
        }

        if (!CollideFaceSomething(boxB.z(), boxA, boxB))
        {
            return false;
        }

        if (!CollideEdgesEdges(boxA, boxB))
        {
            return false;
        }

        return true;
    }

    void intersectionFound(const Intersection & intersection)
    {
        if (numContactPoints < MAX_NUM_CONTACT_POINTS - 1)
        {
           *(Intersection*)&contactPoints[numContactPoints] = intersection;
        }
        else if (contactPoints[numContactPoints - 1].depth < intersection.depth)
        {
            *(Intersection*)&contactPoints[numContactPoints - 1] = intersection;
        }

        std::sort(contactPoints, contactPoints + MAX_NUM_CONTACT_POINTS, compareIntersections);
    }
};

}

namespace deliberation
{

CollideBox3D::CollideBox3D(const Box & boxA,
                           const Box & boxB,
                           CollideBox3DDebugInfo * debugInfo):
    m_a(boxA),
    m_b(boxB),
    m_debugInfo(debugInfo),
    m_minIntersectionDepth(std::numeric_limits<float>::max())
{

}

bool CollideBox3D::execute()
{
    /**
     * Determine intersectionIndex
     */
    if (checkFaceNormalDepth(m_a.box, m_b.box, 0))
    {
        return false;
    }

    if (checkFaceNormalDepth(m_b.box, m_a.box, 3))
    {
        return false;
    }

    {
        uint index = 6;
        for (uint a0 = 0; a0 < 3; a0++)
        {
            for (uint a1 = 0; a1 < 3; a1++)
            {
                if (checkEdgeDepth(m_a.box.axis(a0), m_b.box.axis(a1), index))
                {
                    return false;
                }
                index++;
            }
        }
    }

    //std::cout << "IntersectionIndex: " << m_intersectionIndex << std::endl;

    /**
     *
     */
    m_boxPositionDelta = m_b.box.p() - m_a.box.p();
    m_a.inverseOrientation = glm::transpose(m_a.box.orientationMatrix());
    m_b.inverseOrientation = glm::transpose(m_b.box.orientationMatrix());

    /**
     *
     */
    if (m_intersectionIndex < 6)
    {
        if (m_intersectionIndex < 3)
        {
            checkFaceIntersection(m_a, m_b, m_intersectionIndex);
        }
        else
        {
            checkFaceIntersection(m_a, m_b, m_intersectionIndex - 3);
        }
    }
    else
    {
        auto edgeIntersectionIndex = m_intersectionIndex - 6;
        auto e0 = edgeIntersectionIndex / 3;
        auto e1 = edgeIntersectionIndex % 3;

        checkEdgeIntersection(e0, e1);
    }

    return true;
}

void CollideBox3D::checkFaceIntersection(const BoxWrapper & ref, const BoxWrapper & in, uint a)
{
    if (m_debugInfo)
    {
        m_debugInfo->edgeCollision = false;
    }

    auto & rBox = ref.box;
    auto & iBox = in.box;

    auto direction = rBox.axis(a);
    auto normal = normalFromDirection(direction);
    std::cout << "  Normal: " << normal << std::endl;

    auto rNormal = ref.inverseOrientation * normal;
    auto iNormal = in.inverseOrientation * -normal;

    Face rFace = computeFaceFromNormal(rBox, rNormal);
    Face iFace = computeFaceFromNormal(iBox, iNormal);

    if (m_debugInfo)
    {
        m_debugInfo->incidentFaceCenter = iFace.center;
        m_debugInfo->incidentFaceNormal = iFace.normalAxis;
        m_debugInfo->referenceFaceCenter = rFace.center;
        m_debugInfo->referenceFaceNormal = rFace.normalAxis;
    }

    glm::mat3 rFaceRot = glm::mat3(rFace.xAxis, rFace.yAxis, rFace.normalAxis);
    glm::mat3 invRFaceRot = glm::transpose(rFaceRot);

    /**
     * Project incident face on reference face
     */
    auto x = iFace.xAxis * iBox.halfExtent()[iFace.xAxisIndex];
    auto y = iFace.yAxis * iBox.halfExtent()[iFace.yAxisIndex];

    glm::vec3 in3[4];
    in3[0] = iFace.center + x + y;
    in3[1] = iFace.center - x + y;
    in3[2] = iFace.center - x - y;
    in3[3] = iFace.center + x - y;

    if (m_debugInfo)
    {
        memcpy(m_debugInfo->incidentFace, in3, sizeof(in3));
    }

    glm::vec2 ref2[4];
    {
        auto x = rBox.halfExtent()[rFace.xAxisIndex];
        auto y = rBox.halfExtent()[rFace.yAxisIndex];

        ref2[0] = glm::vec2( x, y);
        ref2[1] = glm::vec2(-x, y);
        ref2[2] = glm::vec2(-x,-y);
        ref2[3] = glm::vec2( x,-y);
    }

    if (m_debugInfo)
    {
        for (uint v = 0; v < 4; v++)
        {
            m_debugInfo->referenceFace[v] = rFaceRot * glm::vec3(ref2[v].x, ref2[v].y, 0) + rFace.center;
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

    if (m_debugInfo)
    {
        m_debugInfo->numClipPoints = result.size();

        for (uint v = 0; v < m_debugInfo->numClipPoints; v++)
        {
            m_debugInfo->clipPoints[v] = rFaceRot * glm::vec3(result[v].x, result[v].y, 0) + rFace.center;
        }
    }

    /**
     * Incident face center and normal in reference face coordinates
     */
    glm::vec3 inCenterRef = invRFaceRot * (iFace.center - rFace.center);
    glm::vec3 inNormalRef = invRFaceRot * iFace.normalAxis;

    /**
     *
     */
    Assert(inNormalRef.z != 0.0f, "");

    Assert(result.size() <= 8, "");

    numIntersections = 0;
    for (auto r = 0u; r < result.size(); r++)
    {
        auto x = result[r].x;
        auto y = result[r].y;
        auto & n = inNormalRef;
        auto nom =  - (x - inCenterRef.x) * n.x - (y - inCenterRef.y) * n.y;

        auto z = (nom / (n.z)) + inCenterRef.z;
        if (z < 0)
        {
            Intersection & intersection = intersections[numIntersections];
            numIntersections++;

            intersection.position = rFaceRot * glm::vec3(result[r].x, result[r].y, 0) + rFace.center;
            intersection.normal = normal;
            intersection.depth = -z;
        }
    }
}

void CollideBox3D::checkEdgeIntersection(uint e0, uint e1)
{
    auto direction = m_edgeCrossProducts[e0 * 3 + e1];

    auto checkDirection = glm::normalize(direction);

    if (m_debugInfo)
    {
        m_debugInfo->edgeCollision = true;
    }

    auto normal = normalFromDirection(checkDirection);
    std::cout << "  Normal2: " << normal << std::endl;

    auto & boxA = m_a.box;
    auto & boxB = m_b.box;

    auto aNormal = m_a.inverseOrientation * normal;
    auto bNormal = m_b.inverseOrientation * -normal;

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

    auto dirA = boxA.axis(e0);
    auto dirB = boxB.axis(e1);

    auto a = 0.0f;
    auto b = 0.0f;
    Ray3DClosestApproach(pointA, dirA, pointB, dirB, a, b);

    auto contactA = pointA + a * dirA;
    auto contactB = pointB + b * dirB;

    auto position = (contactA + contactB) * 0.5f;

    /**
     * Add intersection
     */
    auto & intersection = intersections[0];
    intersection.position = position;
    intersection.normal = normal;
    intersection.depth = glm::length(contactA - contactB);
    numIntersections = 1;
}

bool CollideBox3D::checkFaceNormalDepth(const Box & lhs, const Box & rhs, uint baseIndex)
{
    for (uint a = 0; a < 3; a++)
    {
        auto direction = lhs.axis(a); // |direction| == 1

        auto minB = std::numeric_limits<float>::max();
        auto maxB = std::numeric_limits<float>::lowest();

        auto aProj = glm::dot(direction, lhs.p());
        auto minA = aProj - lhs.halfExtent()[a];
        auto maxA = aProj + lhs.halfExtent()[a];

        for (auto p = 0; p < 8; p++)
        {
            auto r = glm::dot(rhs.point(p), direction);
            minB = std::min(r, minB);
            maxB = std::max(r, maxB);
        }

        auto diffA = maxA - minA;
        auto diffB = maxB - minB;
        auto diffCombined = std::max(maxA, maxB) - std::min(minA, minB);

        auto depth = diffA + diffB - diffCombined;

        if (depth < 0)
        {
            return true;
        }

        if (depth < m_minIntersectionDepth)
        {
            m_minIntersectionDepth = depth;
            m_intersectionIndex = baseIndex + a;
        }
    }

    return false;
}

bool CollideBox3D::checkEdgeDepth(const glm::vec3 & normalA, const glm::vec3 & normalB, uint index)
{
    auto direction = glm::cross(normalA, normalB);
    m_edgeCrossProducts[index - 6] = direction;

    /**
     * normalA || normalB
     */
    if (direction == glm::vec3(0.0f))
    {
        return false;
    }

    auto minA = std::numeric_limits<float>::max();
    auto maxA = std::numeric_limits<float>::lowest();
    auto minB = std::numeric_limits<float>::max();
    auto maxB = std::numeric_limits<float>::lowest();

    for (auto p = 0; p < 8; p++)
    {
        auto r = glm::dot(m_a.box.point(p), direction);
        minA = std::min(r, minA);
        maxA = std::max(r, maxA);

        r = glm::dot(m_b.box.point(p), direction);
        minB = std::min(r, minB);
        maxB = std::max(r, maxB);
    }

    auto diffA = maxA - minA;
    auto diffB = maxB - minB;
    auto diffCombined = std::max(maxA, maxB) - std::min(minA, minB);

    auto depth = diffA + diffB - diffCombined;

    if (depth < m_minIntersectionDepth)
    {
        m_minIntersectionDepth = depth;
        m_intersectionIndex = index;
    }

    return depth < 0;
}

glm::vec3 CollideBox3D::normalFromDirection(const glm::vec3 & direction) const
{
    auto dot = glm::dot(direction, m_boxPositionDelta);

    if (dot < 0)
    {
        return glm::normalize(-direction);
    }
    else
    {
        return glm::normalize(direction);
    }
}

CollideBox3D::BoxWrapper::BoxWrapper(const Box & box):
    box(box)
{
    
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
    CollideBox3D collideBox3D(boxA, boxB);
    m_intersect = collideBox3D.execute();

    /**
     * Update contact points
     */
    if (m_intersect)
    {
        for (uint p = 0; p < collideBox3D.numIntersections; p++)
        {
            auto & intersection = collideBox3D.intersections[p];

            auto index = matchPoint(intersection.position);
            if (index >= 0)
            {
                updatePoint(index, intersection);
            }
            else
            {
                addPoint(intersection);
            }
        }
        m_numPoints = collideBox3D.numIntersections;
    }
}

}
