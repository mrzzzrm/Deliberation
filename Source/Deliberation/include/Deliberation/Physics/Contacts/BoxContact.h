#pragma once

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Physics/Contacts/Contact.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Manifold;
struct CollideBoxBoxDebugInfo;

bool DELIBERATION_API CollideBoxBox(const Box & boxA,
                                    const Box & boxB,
                                    Manifold & manifold,
                                    CollideBoxBoxDebugInfo * debugInfo = nullptr);

struct CollideBoxBoxDebugInfo
{
    bool edgeCollision = false;

    glm::vec3 referenceFace[4];
    glm::vec3 referenceFaceCenter;
    glm::vec3 referenceFaceNormal;

    glm::vec3 incidentFace[4];
    glm::vec3 incidentFaceCenter;
    glm::vec3 incidentFaceNormal;

    u16 numClipPoints = 0;
    glm::vec3 clipPoints[8];
};

class DELIBERATION_API BoxContact final:
    public Contact
{
public:
    BoxContact(RigidBody & bodyA, RigidBody & bodyB);

    virtual void update() override;
};

}