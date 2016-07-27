#pragma once

#include <vector>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Mesh2 final
{
public:
    struct Face
    {
        std::vector<u32> indices;
    };

    using Faces = std::vector<Face>;

public:
    Mesh2(LayoutedBlob && vertices,
          LayoutedBlob && faceAttributes,
          std::vector<Face> && faces);

    const LayoutedBlob & vertices() const;
    const LayoutedBlob & faceAttributes() const;
    const std::vector<Face> & faces() const;

    LayoutedBlobElement faceVertex(size_t face, size_t vertex);
    CLayoutedBlobElement faceVertex(size_t face, size_t vertex) const;

private:
    LayoutedBlob        m_vertices;
    LayoutedBlob        m_faceAttributes;
    std::vector<Face>   m_faces;
};

}