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
    struct Face
    {
        std::vector<std::size_t> indices;
    };

    typedef T Vertex;
    typedef std::vector<Vertex> Vertices;
    typedef std::vector<Face> Faces;

public:
    Mesh(const Vertices & vertices,
         const Faces & faces);

    const Vertices & vertices() const;
    const Faces & faces() const;

    const Vertex & faceVertex(std::size_t face, std::size_t vertex) const;

private:
    Vertices m_vertices;
    Faces m_faces;
};

}

#include <Deliberation/Scene/Mesh.inl>

