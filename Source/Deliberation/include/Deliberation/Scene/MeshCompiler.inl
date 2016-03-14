#include <Deliberation/Core/StreamUtils.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>

#define VERBOSE 0

namespace deliberation
{

template<typename T>
MeshCompiler<T>::MeshCompiler(const Mesh<T> & mesh, PrimitiveType primitive):
    m_mesh(mesh),
    m_primitive(primitive)
{

}

template<typename T>
void MeshCompiler<T>::compile()
{
    m_vertices.clear();
    m_indices.clear();

    switch (m_primitive)
    {
    case Primitive_Triangles: compileTriangles(); break;
    case Primitive_Lines: compileLines(); break;
    default:
        Fail("Unsupported primitive");
    }

#if VERBOSE
    std::cout << "MeshCompiler" << std::endl;
    std::cout << "  Vertices: " << m_vertices.size() << std::endl;
    std::cout << "  Indices: " << m_indices.size() << std::endl;
#endif
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

template<typename T>
void MeshCompiler<T>::compileTriangles()
{
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
void MeshCompiler<T>::compileLines()
{
    for (std::size_t f = 0u; f < m_mesh.faces().size(); f++)
    {
        auto & face = m_mesh.faces()[f];

        for (std::size_t v = 0; v < face.indices.size(); v++)
        {
            m_vertices.push_back(m_mesh.faceVertex(f, v));
            m_vertices.push_back(m_mesh.faceVertex(f, v < face.indices.size() - 1 ? v + 1 : 0));

            m_indices.push_back(m_indices.size());
            m_indices.push_back(m_indices.size());
        }
    }
}

}

#undef VERBOSE
