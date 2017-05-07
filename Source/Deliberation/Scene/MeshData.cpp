#include <Deliberation/Scene/MeshData.h>

namespace deliberation
{
MeshData::MeshData(
    LayoutedBlob &&      vertices,
    LayoutedBlob &&      indices,
    std::vector<Texture> textures)
    : m_vertices(std::move(vertices))
    , m_indices(std::move(indices))
    , m_textures(std::move(textures))
{
}

const LayoutedBlob & MeshData::vertices() const { return m_vertices; }

const LayoutedBlob & MeshData::indices() const { return m_indices; }

const std::vector<Texture> & MeshData::textures() const { return m_textures; }

LayoutedBlob MeshData::takeVertices()
{
    auto result = std::move(m_vertices);
    m_vertices = LayoutedBlob();
    return result;
}

LayoutedBlob MeshData::takeIndices()
{
    auto result = std::move(m_indices);
    m_indices = LayoutedBlob();
    return result;
}
}