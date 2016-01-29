#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API Mesh final
{
public:
    typedef T Vertex;

    struct Face
    {
        std::vector<std::size_t> indices;
    };

public:
    Mesh(const std::vector<Vertex> & vertices,
         const std::vector<Face> & faces);

    const std::vector<Vertex> & vertices() const;
    const std::vector<Face> & faces() const;

    const Vertex & faceVertex(std::size_t face, std::size_t vertex) const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<Face> m_faces;
};

}

#include <Deliberation/Scene/Mesh.inl>

