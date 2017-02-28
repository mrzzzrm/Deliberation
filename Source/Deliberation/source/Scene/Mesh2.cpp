#include <Deliberation/Scene/Mesh2.h>


namespace deliberation
{

Mesh2::Mesh2(LayoutedBlob && vertices, LayoutedBlob && indices, std::vector<Texture> textures):
    m_vertices(std::move(vertices)),
    m_indices(std::move(indices)),
    m_textures(std::move(textures))
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

const std::vector<Texture> & Mesh2::textures() const
{
    return m_textures;
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