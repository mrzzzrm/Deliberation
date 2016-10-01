#pragma once

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Scene/Mesh.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API ConeMesh final
{
public:
    ConeMesh(float radius, float height, uint numSegments = 8);

    Mesh generate() const;

private:
    uint    m_numSegments;
    float   m_radius;
    float   m_height;
};

}