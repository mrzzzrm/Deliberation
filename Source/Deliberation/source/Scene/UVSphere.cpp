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

Mesh2 UVSphere::generate()
{
    Assert(m_numParallels != 0 && m_numMeridians != 0, "");

    DataLayout vertexLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});

    auto numVertices = m_numParallels * m_numMeridians + 2;
    LayoutedBlob vertices(vertexLayout, numVertices);

    auto positions = vertices.field<glm::vec3>("Position");
    auto normals = vertices.field<glm::vec3>("Normal");

    Mesh2::Faces faces;

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
        Mesh2::Face face;
        face.indices = {0u, 1 + ((m + 1) % m_numMeridians), 1u + m};
        faces.push_back(face);
    }

    for (std::size_t p = 0; p < m_numParallels - 2; p++)
    {
        auto baseA = 1u + p * m_numMeridians;
        auto baseB = 1u + (p + 1) * m_numMeridians;

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {
            Mesh2::Face face;
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
        Mesh2::Face face;
        face.indices = {(u32)(vertices.count() - 1), (u32)(a), (u32)(b)};
        faces.push_back(face);
	}

    return Mesh2(std::move(vertices), std::move(faces));
}


}

