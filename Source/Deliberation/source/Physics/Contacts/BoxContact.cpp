#include <Deliberation/Physics/Contacts/BoxContact.h>

#include <iostream>
#include <limits>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Physics/Contacts/Manifold.h>

namespace
{

using namespace deliberation;

struct BoxContactAlgorithm
{

const Box & boxA;
const Box & boxB;
Manifold & manifold;

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
        // @@@ this needs to be made more robust
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

BoxContactAlgorithm(const Box & boxA, const Box & boxB, Manifold & manifold):
    boxA(boxA),
    boxB(boxB),
    manifold(manifold)
{

}

bool CollideFaceSomething(const glm::vec3 & direction, const Box & boxA, const Box & boxB, Manifold & manifold, bool flip)
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
            manifold.setDepth(depth);
            manifold.setNormal(normalFromDirection(checkDirection));
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

            std::cout << aNormal << " " << bNormal << std::endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool CollideFacesSomething(const Box & boxA, const Box & boxB, Manifold & manifold, bool flip)
{
    if (!CollideFaceSomething(boxA.x(), boxA, boxB, manifold, flip))
    {
        std::cout << "No collision in direction " << boxA.x() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxA.y(), boxA, boxB, manifold, flip))
    {
        std::cout << "No collision in direction " << boxA.y() << std::endl;
        return false;
    }

    if (!CollideFaceSomething(boxA.z(), boxA, boxB, manifold, flip))
    {
        std::cout << "No collision in direction " << boxA.z() << std::endl;
        return false;
    }

    return true;
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


    if (!CollideFacesSomething(boxA, boxB, manifold, false))
    {
        return false;
    }

    if (!CollideFacesSomething(boxB, boxA, manifold, true))
    {
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

bool DELIBERATION_API CollideBoxBox(const Box & boxA, const Box & boxB, Manifold & manifold)
{
    BoxContactAlgorithm algorithm(boxA, boxB, manifold);
    return algorithm.execute();
}

BoxContact::BoxContact(RigidBody & bodyA, RigidBody & bodyB):
    Contact(bodyA, bodyB)
{

}

void BoxContact::update()
{

}

}
