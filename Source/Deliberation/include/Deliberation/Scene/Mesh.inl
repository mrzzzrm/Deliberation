#include <string>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
Mesh<T>::Mesh()
{
}

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

    auto index = m_faces[face].indices[vertex];
    Assert(index < m_vertices.size(), "");

    return m_vertices[index];
}

template<typename T>
void Mesh<T>::computeNormals()
{
    /*
        TODO
            Only do this when necessary (i.e. faces not already separate)
    */
    separateFaces();

    for (auto & face : m_faces)
    {
        Assert(face.indices.size() >= 3, "");
        auto normal = glm::normalize(glm::cross(m_vertices[face.indices[1]].position - m_vertices[face.indices[2]].position,
                                                m_vertices[face.indices[1]].position - m_vertices[face.indices[0]].position));


        for (auto & index : face.indices)
        {
            m_vertices[index].normal = normal;
        }
    }
}

template<typename T>
void Mesh<T>::separateFaces()
{
    Vertices newVertices;
    Faces newFaces;

    for (auto & face : m_faces)
    {
        Face newFace;

        for (auto & index : face.indices)
        {
            newFace.indices.push_back(newVertices.size());
            newVertices.push_back(m_vertices[index]);
        }

        newFaces.push_back(newFace);
    }

    m_vertices = std::move(newVertices);
    m_faces = std::move(newFaces);
}

}


