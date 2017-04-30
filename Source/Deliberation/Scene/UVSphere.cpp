#include <Deliberation/Scene/UVSphere.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{

UVSphere::UVSphere():
    m_numParallels(0),
    m_numMeridians(0)
{

}

UVSphere::UVSphere(unsigned int numParallels, unsigned int numMeridians):
    m_numParallels(numParallels),
    m_numMeridians(numMeridians)
{

}

Mesh UVSphere::generateMesh() const
{
    Assert(m_numParallels != 0 && m_numMeridians != 0, "");

    DataLayout vertexLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});

    auto numVertices = m_numParallels * m_numMeridians + 2;
    LayoutedBlob vertices(vertexLayout, numVertices);

    auto positions = vertices.field<glm::vec3>("Position");
    auto normals = vertices.field<glm::vec3>("Normal");

    Mesh::Faces faces;

    positions[0] = {0.0f, 1.0f, 0.0f};

    for (std::size_t p = 0; p < m_numParallels; p++)
    {
		float polar = glm::pi<float>() * float(p + 1) / m_numParallels;
		float sp = std::sin(polar);
		float cp = std::cos(polar);

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {
			float azimuth = 2.0f * glm::pi<float>() * float(m) / m_numMeridians;
			float sa = std::sin(azimuth);
			float ca = std::cos(azimuth);

            positions[p * m_numMeridians + m] = {sp * ca, cp, sp * sa};
        }
    }

    positions[numVertices - 1] = {0.0f, -1.0f, 0.0f};

    for (auto n = 0; n < numVertices; n++)
    {
        normals[n] = glm::normalize(positions[n]);
    }

    for (auto m = 0; m < m_numMeridians; m++)
    {
        Mesh::Face face;
        face.indices = {0u, 1 + ((m + 1) % m_numMeridians), 1u + m};
        faces.push_back(face);
    }

    for (std::size_t p = 0; p < m_numParallels - 2; p++)
    {
        auto baseA = 1u + p * m_numMeridians;
        auto baseB = 1u + (p + 1) * m_numMeridians;

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {
            Mesh::Face face;
            face.indices = {(u32)(baseA + m),
                            (u32)(baseA + (m + 1) % m_numMeridians),
                            (u32)(baseB + (m + 1) % m_numMeridians),
                            (u32)(baseB + m)};
            faces.push_back(face);
        }
    }

	for (std::size_t m = 0; m < m_numMeridians; m++)
	{
		auto a = m + m_numMeridians * (m_numParallels - 2) + 1;
		auto b = (m + 1) % m_numMeridians + m_numMeridians * (m_numParallels - 2) + 1;
        Mesh::Face face;
        face.indices = {(u32)(vertices.count() - 1), (u32)(a), (u32)(b)};
        faces.push_back(face);
	}

    return Mesh(std::move(vertices), std::move(faces));
}

MeshData UVSphere::generateMesh2() const
{
    Assert(m_numParallels != 0 && m_numMeridians != 0, "");

    DataLayout vertexLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});

    auto numVertices = m_numParallels * m_numMeridians + 2;
    LayoutedBlob vertices(vertexLayout, numVertices);

    auto positions = vertices.field<glm::vec3>("Position");
    auto normals = vertices.field<glm::vec3>("Normal");

    Mesh::Faces faces;

    positions[0] = {0.0f, 1.0f, 0.0f};

    /**
     * Generate vertices
     */
    for (std::size_t p = 0; p < m_numParallels; p++)
    {
        float polar = glm::pi<float>() * float(p + 1) / m_numParallels;
        float sp = std::sin(polar);
        float cp = std::cos(polar);

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {
            float azimuth = 2.0f * glm::pi<float>() * float(m) / m_numMeridians;
            float sa = std::sin(azimuth);
            float ca = std::cos(azimuth);

            positions[p * m_numMeridians + m + 1] = {sp * ca, cp, sp * sa};
        }
    }

    positions[numVertices - 1] = {0.0f, -1.0f, 0.0f};

    for (auto n = 0; n < numVertices; n++)
    {
        normals[n] = glm::normalize(positions[n]);
    }

    /**
     * Generate indices
     */
    auto numIndices = m_numMeridians * 3 +
        (m_numParallels - 2) * m_numMeridians * 6 +
        m_numMeridians * 3;
    DataLayout indexLayout("Index", Type_U32);
    LayoutedBlob indexBlob(indexLayout, numIndices);
    auto indices = indexBlob.field<u32>("Index");
    u32 i = 0;

    for (auto m = 0; m < m_numMeridians; m++)
    {
        indices[i++] = 0u;
        indices[i++] = 1 + ((m + 1) % m_numMeridians);
        indices[i++] = 1u + m;
    }

    for (std::size_t p = 0; p < m_numParallels - 2; p++)
    {
        auto baseA = 1u + p * m_numMeridians;
        auto baseB = 1u + (p + 1) * m_numMeridians;

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {
            indices[i++] = (u32)(baseA + m);
            indices[i++] = (u32)(baseA + (m + 1) % m_numMeridians);
            indices[i++] = (u32)(baseB + (m + 1) % m_numMeridians);

            indices[i++] = (u32)(baseA + m);
            indices[i++] = (u32)(baseB + (m + 1) % m_numMeridians);
            indices[i++] = (u32)(baseB + m);
        }
    }

    for (std::size_t m = 0; m < m_numMeridians; m++)
    {
        auto a = m + m_numMeridians * (m_numParallels - 2) + 1;
        auto b = (m + 1) % m_numMeridians + m_numMeridians * (m_numParallels - 2) + 1;

        indices[i++] = (u32)(vertices.count() - 1);
        indices[i++] = (u32)(a);
        indices[i++] = (u32)(b);
    }

    return MeshData(std::move(vertices), std::move(indexBlob));
}

}

