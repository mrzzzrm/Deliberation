#pragma once

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Scene/Mesh.h>

namespace deliberation
{
class ConeMesh final
{
public:
    ConeMesh(float radius, float height, uint numSegments = 8);

    Mesh generate() const;

private:
    uint  m_numSegments;
    float m_radius;
    float m_height;
};
}