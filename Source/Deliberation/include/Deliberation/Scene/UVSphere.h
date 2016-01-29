#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Scene/Mesh.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API UVSphere
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

public:
    UVSphere();
    UVSphere(unsigned int numParallels, unsigned int numMeridians);

    Mesh<Vertex> generate();

private:
    unsigned int m_numParallels;
    unsigned int m_numMeridians;
};

}

