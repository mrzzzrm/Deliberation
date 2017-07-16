#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{
class Mesh;

class EdgeToFaceSmoothing final
{
public:
    EdgeToFaceSmoothing(const Mesh & sourceMesh);

    Mesh run();

private:
    const Mesh & m_sourceMesh;
};
}