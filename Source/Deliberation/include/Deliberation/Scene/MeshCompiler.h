#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Scene/Mesh.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API MeshCompiler final
{
public:
    MeshCompiler(const Mesh<T> & mesh);

    void compile();

    const std::vector<typename Mesh<T>::Vertex> & vertices() const;
    const std::vector<unsigned int> & indices() const;

private:
    const Mesh<T> &                        m_mesh;
    std::vector<typename Mesh<T>::Vertex>  m_vertices;
    std::vector<unsigned int>              m_indices;
};

}

#include <Deliberation/Scene/MeshCompiler.inl>
