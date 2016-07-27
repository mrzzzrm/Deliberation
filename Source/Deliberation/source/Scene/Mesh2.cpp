#include <Deliberation/Scene/Mesh2.h>

#include <functional>

namespace deliberation
{

Mesh2::Mesh2(LayoutedBlob && vertexAttributes,
             LayoutedBlob && faceAttributes,
             std::vector<Face> && faces):
    m_vertexAttributes(std::move(vertexAttributes)),
    m_faceAttributes(std::move(faceAttributes)),
    m_faces(std::move(faces))
{

}

}