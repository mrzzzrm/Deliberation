#include <Deliberation/Scene/EdgeToFaceSmoothing.h>

#include <iostream>
#include <unordered_map>

#include <Deliberation/Core/HashUtils.h>

#include <Deliberation/Scene/Mesh.h>

namespace
{
using namespace deliberation;

static const u32 MAX_NUM_EDGES_PER_VERTEX = 8;

struct Vertex
{
    u32 original;
    u32 edges[MAX_NUM_EDGES_PER_VERTEX];
    u32 descendants[MAX_NUM_EDGES_PER_VERTEX];

    Vertex()
    {
        original = (u32)-1;
        for (auto i = 0; i < MAX_NUM_EDGES_PER_VERTEX; i++)
        {
            edges[i] = (u32)-1;
            descendants[i] = (u32)-1;
        }
    }
};

struct Edge
{
    u32 vertices[2];

    Edge() = default;

    Edge(u32 a, u32 b)
    {
        vertices[0] = a;
        vertices[1] = b;
    }
};

struct Face
{
    std::vector<u32> vertices;
};
}

namespace deliberation
{
EdgeToFaceSmoothing::EdgeToFaceSmoothing(const Mesh & sourceMesh)
    : m_sourceMesh(sourceMesh)
{
}

Mesh EdgeToFaceSmoothing::run()
{
    std::unordered_map<size_t, Edge> edges;

    for (auto f = 0; f < m_sourceMesh.faces().size(); f++)
    {
        auto & face = m_sourceMesh.faces().at(f);

        for (auto v0 = 0; v0 < face.indices.size(); v0++)
        {
            auto v1 = (v0 + 1) % face.indices.size();

            auto i0 = face.indices[v0];
            auto i1 = face.indices[v1];

            auto hash = i0 < i1 ? HashCombine(i0, i1) : HashCombine(i1, i0);

            edges[HashCombine(i0, i1)] = Edge(i0, i1);
        }
    }

    std::cout << "Edges: " << edges.size() << std::endl;
    for (auto & pair : edges)
    {
        std::cout << " " << pair.second.vertices[0] << " "
                  << pair.second.vertices[1] << std::endl;
    }

    return Mesh();
}
}