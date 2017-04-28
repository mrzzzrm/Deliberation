#pragma once

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Physics/Contacts/Contact.h>



namespace deliberation
{

class Manifold;
struct CollideBox3DDebugInfo;

struct CollideBox3DDebugInfo
{
    bool edgeCollision = false;

    glm::vec3 referenceFace[4];
    glm::vec3 referenceFaceCenter;
    glm::vec3 referenceFaceNormal;

    glm::vec3 incidentFace[4];
    glm::vec3 incidentFaceCenter;
    glm::vec3 incidentFaceNormal;

    uint numClipPoints = 0;
    glm::vec3 clipPoints[8];
};

class CollideBox3D
{
public:
    CollideBox3D(const Box & boxA,
                 const Box & boxB,
                 CollideBox3DDebugInfo * debugInfo = nullptr);

    bool execute();

public:
    uint         numIntersections;
    Intersection intersections[8];

private:
    struct BoxWrapper
    {
        const Box & box;
        glm::mat3 inverseOrientation;

        BoxWrapper(const Box & box);
    };

private:
    bool checkFaceNormalDepth(const Box & lhs, const Box & rhs, uint baseIndex);
    bool checkEdgeDepth(const glm::vec3 & normalA, const glm::vec3 & normalB, uint index);
    void checkFaceIntersection(const BoxWrapper & ref, const BoxWrapper & in, const glm::vec3 & direction, bool flip);
    void checkEdgeIntersection(uint e0, uint e1);
    glm::vec3 normalFromDirection(const glm::vec3 & direction) const;

private:
    BoxWrapper                  m_a;
    BoxWrapper                  m_b;

    CollideBox3DDebugInfo *     m_debugInfo;


    /**
     * 0..2 x,y,z of boxA
     * 3..5 x,y,z of boxB
     *
     * 6..8 x of boxA and x,y,z of boxB
     * 9..11 y of boxA and x,y,z of boxB
     * 12..4 z of boxA and x,y,z of boxB
     */
    int                         m_intersectionIndex;
    float                       m_minIntersectionDepth;

    glm::vec3                   m_refCenterToInCenter;

    glm::vec3                   m_edgeCrossProducts[9];
};

class BoxContact final:
    public Contact
{
public:
    BoxContact(RigidBody & bodyA, RigidBody & bodyB);

    virtual void update() override;
};

}