#include <Deliberation/Scene/UVSphere.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Deliberation/Core/Assert.h>

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

Mesh<UVSphere::Vertex> UVSphere::generate()
{
    Assert(m_numParallels != 0 && m_numMeridians != 0, "");

    std::vector<Vertex> vertices;
    std::vector<Mesh<Vertex>::Face> faces;

    Vertex north;
    north.position = {0.0f, 1.0f, 0.0f};
    vertices.push_back(north);

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
			float x = sp * ca;
			float y = cp;
			float z = sp * sa;

            Vertex vertex;
            vertex.position.x = sp * ca;
            vertex.position.y = cp;
            vertex.position.z = sp * sa;

			vertices.push_back(vertex);
        }
    }

    Vertex south;
    south.position = {0.0f, -1.0f, 0.0f};
    vertices.push_back(south);


    for (auto & vertex : vertices)
    {
        vertex.normal = glm::normalize(vertex.position);
    }

    for (std::size_t m = 0; m < m_numMeridians; m++)
    {
        Mesh<Vertex>::Face face;
        face.indices = {0, 1 + ((m + 1) % m_numMeridians), 1 + m};
        faces.push_back(face);
    }

    for (std::size_t p = 0; p < m_numParallels - 2; p++)
    {
        auto baseA = 1 + p * m_numMeridians;
        auto baseB = 1 + (p + 1) * m_numMeridians;

        for (std::size_t m = 0; m < m_numMeridians; m++)
        {

            Mesh<Vertex>::Face face;
            face.indices = {baseA + m, baseA + (m + 1) % m_numMeridians,
                            baseB + (m + 1) % m_numMeridians, baseB + m};
            faces.push_back(face);
        }
    }

	for (std::size_t m = 0; m < m_numMeridians; m++)
	{
		auto a = m + m_numMeridians * (m_numParallels - 2) + 1;
		auto b = (m + 1) % m_numMeridians + m_numMeridians * (m_numParallels - 2) + 1;
        Mesh<Vertex>::Face face;
        face.indices = {vertices.size() - 1, a, b};
        faces.push_back(face);
	}

    return Mesh<Vertex>(vertices, faces);
}


}

