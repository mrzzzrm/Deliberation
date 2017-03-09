#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Scene/Mesh.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API CuboidMesh final
{
public:
    static std::vector<glm::vec3> positions(const glm::vec3 & size);
    static std::vector<u32> indices();

public:
    CuboidMesh(const glm::vec3 & size);

    Mesh generate() const;

private:
    glm::vec3 m_size;
};

}