#include <Deliberation/Scene/Mesh2.h>

namespace deliberation
{

Mesh2::Mesh2(LayoutedBlob && vertices, LayoutedBlob && indices):
    m_vertices(std::move(vertices)),
    m_indices(std::move(indices))
{

}

const LayoutedBlob & Mesh2::vertices() const
{
    return m_vertices;
}

const LayoutedBlob & Mesh2::indices() const
{
    return m_indices;
}

LayoutedBlob Mesh2::takeVertices()
{
    auto result = std::move(m_vertices);
    m_vertices = LayoutedBlob();
    return result;
}

LayoutedBlob Mesh2::takeIndices()
{
    auto result = std::move(m_indices);
    m_indices = LayoutedBlob();
    return result;
}

}