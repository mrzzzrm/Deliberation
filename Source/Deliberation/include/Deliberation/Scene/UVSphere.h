#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Scene/Mesh2.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API UVSphere
{
public:
    UVSphere();
    UVSphere(unsigned int numParallels, unsigned int numMeridians);

    Mesh2 generate();

private:
    unsigned int m_numParallels;
    unsigned int m_numMeridians;
};

}

