#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Mesh;

class DELIBERATION_API EdgeToFaceSmoothing final
{
public:
    EdgeToFaceSmoothing(const Mesh & sourceMesh);

    Mesh run();

private:
    void generateVertices();
    void generateEdges();

private:
    const Mesh & m_sourceMesh;
};

}