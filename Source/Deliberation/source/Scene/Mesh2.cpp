#include <Deliberation/Scene/Mesh2.h>

#include <functional>

namespace deliberation
{

Mesh2::Mesh2(LayoutedBlob && vertices,
             LayoutedBlob && faceAttributes,
             std::vector<Face> && faces):
    m_vertices(std::move(vertices)),
    m_faceAttributes(std::move(faceAttributes)),
    m_faces(std::move(faces))
{
}

const LayoutedBlob & Mesh2::vertices() const
{
    return m_vertices;
}

const LayoutedBlob & Mesh2::faceAttributes() const
{
    return m_faceAttributes;
}

const std::vector<Mesh2::Face> & Mesh2::faces() const
{
    return m_faces;
}

LayoutedBlobElement Mesh2::faceVertex(size_t face, size_t vertex)
{
    auto e = ((const Mesh2*)this)->faceVertex(face, vertex);
    return LayoutedBlobElement((LayoutedBlob&)e.blob(), e.index());
}

CLayoutedBlobElement Mesh2::faceVertex(size_t face, size_t vertex) const
{
    Assert(face < m_faces.size(), "Out of range: " + std::to_string(face));
    Assert(vertex < m_faces[face].indices.size(), "Out of range: " + std::to_string(vertex));

    auto index = m_faces[face].indices[vertex];
    Assert(index < m_vertices.count(), "");

    return m_vertices[index];
}

}