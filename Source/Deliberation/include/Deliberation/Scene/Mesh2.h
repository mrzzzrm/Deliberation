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

public:
    Mesh2(LayoutedBlob && vertexAttributes,
          LayoutedBlob && faceAttributes,
          std::vector<Face> && faces);

private:
    LayoutedBlob        m_vertexAttributes;
    LayoutedBlob        m_faceAttributes;
    std::vector<Face>   m_faces;
};

}