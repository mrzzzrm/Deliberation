#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

template<typename T>
MeshCompiler<T>::MeshCompiler(const Mesh<T> & mesh):
    m_mesh(mesh)
{

}

template<typename T>
void MeshCompiler<T>::compile()
{
    m_vertices.clear();
    m_indices.clear();

    for (std::size_t f = 0u; f < m_mesh.faces().size(); f++)
    {
        auto & face = m_mesh.faces()[f];

        for (std::size_t v = 1; v  < face.indices.size() - 1; v++)
        {
            m_vertices.push_back(m_mesh.faceVertex(f, 0));
            m_vertices.push_back(m_mesh.faceVertex(f, v));
            m_vertices.push_back(m_mesh.faceVertex(f, v + 1));

            m_indices.push_back(m_indices.size());
            m_indices.push_back(m_indices.size());
            m_indices.push_back(m_indices.size());
        }
    }
}

template<typename T>
const std::vector<typename Mesh<T>::Vertex> & MeshCompiler<T>::vertices() const
{
    return m_vertices;
}

template<typename T>
const std::vector<unsigned int> & MeshCompiler<T>::indices() const
{
    return m_indices;
}

}

