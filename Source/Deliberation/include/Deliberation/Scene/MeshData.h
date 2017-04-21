#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API MeshData
{
public:
    MeshData() = default;
    MeshData(LayoutedBlob && vertices, LayoutedBlob && indices, std::vector<Texture> textures = {});

    const LayoutedBlob & vertices() const;
    const LayoutedBlob & indices() const;
    const std::vector<Texture> & textures() const;

    LayoutedBlob takeVertices();
    LayoutedBlob takeIndices();

private:
    LayoutedBlob            m_vertices;
    LayoutedBlob            m_indices;
    std::vector<Texture>    m_textures;
};

}