#include <string>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
Mesh<T>::Mesh(const Vertices & vertices,
              const Faces & faces):
    m_vertices(vertices),
    m_faces(faces)
{

}

template<typename T>
const typename Mesh<T>::Vertices & Mesh<T>::vertices() const
{
    return m_vertices;
}

template<typename T>
const typename Mesh<T>::Faces & Mesh<T>::faces() const
{
    return m_faces;
}

template<typename T>
const typename Mesh<T>::Vertex & Mesh<T>::faceVertex(std::size_t face, std::size_t vertex) const
{
    Assert(face < m_faces.size(), "Out of range: " + std::to_string(face));
    Assert(vertex < m_faces[face].indices.size(), "Out of range: " + std::to_string(vertex));

    return m_vertices[m_faces[face].indices[vertex]];
}


}


