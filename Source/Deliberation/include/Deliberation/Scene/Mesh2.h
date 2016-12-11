#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Mesh2
{
public:
    Mesh2() = default;
    Mesh2(LayoutedBlob && vertices, LayoutedBlob && indices);

    const LayoutedBlob & vertices() const;
    const LayoutedBlob & indices() const;

    LayoutedBlob && takeVertices();
    LayoutedBlob && takeIndices();

private:
    LayoutedBlob m_vertices;
    LayoutedBlob m_indices;
};

}