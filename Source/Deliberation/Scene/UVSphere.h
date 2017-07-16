#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Scene/Mesh.h>
#include <Deliberation/Scene/MeshData.h>
#include <Deliberation/Scene/Vertices.h>

namespace deliberation
{
class UVSphere
{
public:
    UVSphere();
    UVSphere(unsigned int numParallels, unsigned int numMeridians);

    Mesh     generateMesh() const;
    MeshData generateMesh2() const;

private:
    unsigned int m_numParallels;
    unsigned int m_numMeridians;
};
}
