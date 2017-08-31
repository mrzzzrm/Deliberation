#include <Deliberation/Scene/Mesh.h>

#include <functional>

namespace deliberation
{
Mesh::Face::Face(std::vector<u32> && indices) : indices(std::move(indices)) {}

Mesh::Mesh() = default;

Mesh::Mesh(
    LayoutedBlob &&      vertices,
    LayoutedBlob &&      faceAttributes,
    std::vector<Face> && faces)
    : m_vertices(std::move(vertices))
    , m_faceAttributes(std::move(faceAttributes))
    , m_faces(std::move(faces))
{
}

Mesh::Mesh(LayoutedBlob && vertices, std::vector<Face> && faces)
    : m_vertices(std::move(vertices)), m_faces(std::move(faces))
{
}

LayoutedBlob & Mesh::vertices() { return m_vertices; }

const LayoutedBlob & Mesh::vertices() const { return m_vertices; }

LayoutedBlob & Mesh::faceAttributes() { return m_faceAttributes; }

const LayoutedBlob & Mesh::faceAttributes() const { return m_faceAttributes; }

std::vector<Mesh::Face> & Mesh::faces() { return m_faces; }

const std::vector<Mesh::Face> & Mesh::faces() const { return m_faces; }

LayoutedBlobElement Mesh::faceVertex(size_t face, size_t vertex)
{
    AssertM(face < m_faces.size(), "Out of range: " + std::to_string(face));
    AssertM(
        vertex < m_faces[face].indices.size(),
        "Out of range: " + std::to_string(vertex));

    auto index = m_faces[face].indices[vertex];
    Assert(index < m_vertices.count());

    return m_vertices[index];
}

CLayoutedBlobElement Mesh::faceVertex(size_t face, size_t vertex) const
{
    AssertM(face < m_faces.size(), "Out of range: " + std::to_string(face));
    AssertM(
        vertex < m_faces[face].indices.size(),
        "Out of range: " + std::to_string(vertex));

    auto index = m_faces[face].indices[vertex];
    Assert(index < m_vertices.count());

    return m_vertices[index];
}
}