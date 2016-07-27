#include <Deliberation/Scene/CuboidMesh.h>

namespace deliberation
{
//
//CuboidMesh::CuboidMesh(const glm::vec3 & size):
//    m_size(size)
//{
//
//}
//
//Mesh2 CuboidMesh::generate() const
//{
//    auto vertexLayout = deliberation::DataLayout({
//                                                     {"Position", deliberation::Type_Vec3}
//                                                 });
//    auto faceLayout = deliberation::DataLayout({
//                                                   {"Normal", deliberation::Type_Vec3}
//                                               });
//
//    LayoutedBlob vertices(vertexLayout, 8);
//    LayoutedBlob face_data(faceLayout, 6);
//
//    auto positions = vertices.field<glm::vec3>("Position");
//    auto normals = face_data.field<glm::vec3>("Normal");
//
//    auto hw = size.x / 2.0f;
//    auto hh = size.y / 2.0f;
//    auto hd = size.z / 2.0f;
//
//    positions[0] = {-hw, -hh,  hd};
//    positions[1] = {-hw, -hh, -hd};
//    positions[2] = { hw, -hh, -hd};
//    positions[3] = { hw, -hh,  hd};
//    positions[4] = {-hw,  hh,  hd};
//    positions[5] = { hw,  hh,  hd};
//    positions[6] = { hw,  hh, -hd};
//    positions[7] = {-hw,  hh, -hd};
//
//    normals[0] = {-1.0f, 0.0f, 0.0f};
//    normals[1] = { 1.0f, 0.0f, 0.0f};
//    normals[2] = { 0.0f,-1.0f, 0.0f};
//    normals[3] = { 0.0f, 1.0f, 0.0f};
//    normals[4] = { 0.0f, 0.0f,-1.0f};
//    normals[5] = { 0.0f, 0.0f, 1.0f};
//
//    std::vector<Mesh2::Face> faces({
//        {0, 4, 7, 1}, // l
//        {6, 5, 3, 2}, // r
//        {0, 1, 2, 3}, // b
//        {4, 5, 6, 7}, // r
//        {1, 7, 6, 2}, // b
//        {0, 3, 5, 4}
//    });
//
//    return Mesh2(std::move(vertices),
//                 std::move(face_data),
//                 std::move(faces));
//}

}