#include <Deliberation/Scene/ConeMesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace deliberation
{

ConeMesh::ConeMesh(float radius, float height, uint numSegments):
    m_radius(radius),
    m_height(height),
    m_numSegments(numSegments)
{

}

Mesh ConeMesh::generate() const
{
    Assert(m_radius > 0 && m_height > 0, "");

    DataLayout vertexLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});

    auto numVertices = m_numSegments * 4;

    LayoutedBlob vertices(vertexLayout, numVertices);
    auto positions = vertices.field<glm::vec3>("Position");
    auto normals = vertices.field<glm::vec3>("Normal");

    Mesh::Faces faces;

    /**
     * Add sides
     */
    for (uint s = 0; s < m_numSegments; s++)
    {
        auto r0 = (s * glm::two_pi<float>()) / m_numSegments;
        auto r1 = ((s + 1) * glm::two_pi<float>()) / m_numSegments;

        glm::vec3 a(std::cos(r0) * m_radius, 0.0f, std::sin(r0) * m_radius);
        glm::vec3 b(std::cos(r1) * m_radius, 0.0f, std::sin(r1) * m_radius);
        glm::vec3 c(0.0f, m_height, 0.0f);

        glm::vec3 n = (a + b) / 2.0f;
        n.y = m_radius * glm::length(n) / m_height;
        n = glm::normalize(n);

        auto baseIndex = s * 3;
        positions[baseIndex + 0] = a;
        positions[baseIndex + 1] = c;
        positions[baseIndex + 2] = b;

        normals[baseIndex + 0] = n;
        normals[baseIndex + 1] = n;
        normals[baseIndex + 2] = n;

        Mesh::Face face;
        face.indices = {baseIndex + 0, baseIndex + 1, baseIndex + 2};

        faces.push_back(face);
    }

    /**
     * Add bottom
     */
    Mesh::Face bottom;
    for (uint s = 0; s < m_numSegments; s++)
    {
        auto r = (s * glm::two_pi<float>()) / m_numSegments;

        glm::vec3 p(std::cos(r) * m_radius, 0.0f, std::sin(r) * m_radius);
        glm::vec3 n = glm::vec3(0.0f, -1.0f, 0.0f);

        auto i = m_numSegments * 3 + s;
        positions[i] = p;
        normals[i] = n;

        bottom.indices.push_back(i);
    }

    faces.push_back(bottom);

    return Mesh(std::move(vertices), std::move(faces));
}

}