#pragma once

#include <vector>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Mesh final
{
public:
    struct Face
    {
        Face() = default;
        Face(const Face & rhs) = default;
        Face(std::vector<u32> && indices);

        Face & operator=(const Face & rhs) = default;

        std::vector<u32> indices;
    };

    using Faces = std::vector<Face>;

public:
    Mesh();
    Mesh(LayoutedBlob && vertices,
          std::vector<Face> && faces);
    Mesh(LayoutedBlob && vertices,
          LayoutedBlob && faceAttributes,
          std::vector<Face> && faces);

    LayoutedBlob & vertices();
    const LayoutedBlob & vertices() const;

    LayoutedBlob & faceAttributes();
    const LayoutedBlob & faceAttributes() const;

    std::vector<Face> & faces();
    const std::vector<Face> & faces() const;

    LayoutedBlobElement faceVertex(size_t face, size_t vertex);
    CLayoutedBlobElement faceVertex(size_t face, size_t vertex) const;

private:
    LayoutedBlob        m_vertices;
    LayoutedBlob        m_faceAttributes;
    std::vector<Face>   m_faces;
};

}