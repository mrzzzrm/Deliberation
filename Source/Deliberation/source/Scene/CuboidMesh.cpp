#include <Deliberation/Scene/CuboidMesh.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{

std::vector<glm::vec3> CuboidMesh::positions(const glm::vec3 & size)
{
    std::vector<glm::vec3> positions(8);
    
    auto hw = size.x / 2.0f;
    auto hh = size.y / 2.0f;
    auto hd = size.z / 2.0f;

    positions[0] = {-hw, -hh,  hd};
    positions[1] = {-hw, -hh, -hd};
    positions[2] = { hw, -hh, -hd};
    positions[3] = { hw, -hh,  hd};
    positions[4] = {-hw,  hh,  hd};
    positions[5] = { hw,  hh,  hd};
    positions[6] = { hw,  hh, -hd};
    positions[7] = {-hw,  hh, -hd};

    return positions;
}

std::vector<u32> CuboidMesh::indices() 
{
    return {
        0, 4, 7, 0, 7, 1, // l
        6, 5, 3, 6, 3, 2, // r
        0, 1, 2, 0, 2, 3, // b
        4, 5, 6, 4, 6, 7, // r
        1, 7, 6, 1, 6, 2, // b
        0, 3, 5, 0, 5, 4};
}

CuboidMesh::CuboidMesh(const glm::vec3 & size):
    m_size(size)
{

}

Mesh CuboidMesh::generate() const
{
    auto vertexLayout = DataLayout("Position", deliberation::Type_Vec3);
    auto faceLayout = DataLayout("Normal", deliberation::Type_Vec3);

    LayoutedBlob vertices(vertexLayout, 8);
    LayoutedBlob face_data(faceLayout, 6);

    auto positions = vertices.field<glm::vec3>("Position");
    auto normals = face_data.field<glm::vec3>("Normal");

    auto hw = m_size.x / 2.0f;
    auto hh = m_size.y / 2.0f;
    auto hd = m_size.z / 2.0f;

    positions[0] = {-hw, -hh,  hd};
    positions[1] = {-hw, -hh, -hd};
    positions[2] = { hw, -hh, -hd};
    positions[3] = { hw, -hh,  hd};
    positions[4] = {-hw,  hh,  hd};
    positions[5] = { hw,  hh,  hd};
    positions[6] = { hw,  hh, -hd};
    positions[7] = {-hw,  hh, -hd};

    normals[0] = {-1.0f, 0.0f, 0.0f};
    normals[1] = { 1.0f, 0.0f, 0.0f};
    normals[2] = { 0.0f,-1.0f, 0.0f};
    normals[3] = { 0.0f, 1.0f, 0.0f};
    normals[4] = { 0.0f, 0.0f,-1.0f};
    normals[5] = { 0.0f, 0.0f, 1.0f};

    std::vector<Mesh::Face> faces({
        {{0, 4, 7, 1}}, // l
        {{6, 5, 3, 2}}, // r
        {{0, 1, 2, 3}}, // b
        {{4, 5, 6, 7}}, // r
        {{1, 7, 6, 2}}, // b
        {{0, 3, 5, 4}}
    });

    return Mesh(std::move(vertices),
                std::move(face_data),
                std::move(faces));
}

}