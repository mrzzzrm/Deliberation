#include <Deliberation/Voxel/VoxelClusterMarchingCubes.h>

#include <bitset>

#include <Deliberation/Core/DataLayout.h>

namespace
{

struct Vertex
{
    Vertex(const glm::vec3 &position, const glm::vec3 &normal) : position(position), normal(normal)
    {}

    glm::vec3 position;
    glm::vec3 normal;
};

const std::vector<Vertex> CONFIGS[] = {
    {{{0, 0, 0}, {1, 1, 1}}, {{2, 2, 2}, {3, 3, 3}}}
};

}

namespace deliberation
{

VoxelClusterMarchingCubes::VoxelClusterMarchingCubes(const VoxelCluster<glm::vec3> &cluster) :
    m_cluster(cluster)
{

}

void VoxelClusterMarchingCubes::run()
{
    auto vertexLayout = DataLayout({{"Position", Type_Vec3},
                                    {"Normal",   Type_Vec3},
                                    {"Color",    Type_Vec3}});

    m_vertices = LayoutedBlob(vertexLayout);

    m_positions.reset(m_vertices.field<glm::vec3>("Position"));
    m_normals.reset(m_vertices.field<glm::vec3>("Normal"));
    m_colors.reset(m_vertices.field<glm::vec3>("Color"));

    auto &size = m_cluster.size();
    auto config = std::bitset<8>();

    for (i32 z = 0; z <= size.z; z++)
    {
        for (i32 y = 0; y <= size.y; y++)
        {
            for (i32 x = 0; x <= size.x; x++)
            {
                config.reset();

                config.set(0, checkVoxel(x - 1, y - 1, z - 1));
                config.set(1, checkVoxel(x - 0, y - 1, z - 1));
                config.set(2, checkVoxel(x - 0, y - 1, z - 0));
                config.set(3, checkVoxel(x - 1, y - 1, z - 0));
                config.set(4, checkVoxel(x - 1, y - 0, z - 1));
                config.set(5, checkVoxel(x - 0, y - 0, z - 1));
                config.set(6, checkVoxel(x - 0, y - 0, z - 0));
                config.set(7, checkVoxel(x - 1, y - 0, z - 0));

                generateMesh(x, y, z, config.to_ullong());
            }
        }
    }
}

LayoutedBlob &&VoxelClusterMarchingCubes::takeVertices()
{
    return std::move(m_vertices);
}

bool VoxelClusterMarchingCubes::checkVoxel(i32 x, i32 y, i32 z) const
{
    auto &size = m_cluster.size();

    if (x < 0 || y < 0 || z < 0 ||
        x >= size.x || y >= size.y || z >= size.z)
    {
        return false;
    }

    return m_cluster.test({x, y, z});
}

void VoxelClusterMarchingCubes::generateMesh(i32 x, i32 y, i32 z, u8 configID)
{
    auto &config = CONFIGS[configID];

    if (config.empty())
    {
        return;
    }

    auto vertexIndex = m_vertices.count();

    m_vertices.resize(m_vertices.count() + config.size());

    for (auto &configVertex : config)
    {
        (*m_positions)[vertexIndex] = configVertex.position + glm::vec3(x, y, z);
        (*m_normals)[vertexIndex] = configVertex.normal;
        (*m_colors)[vertexIndex] = glm::vec3(100, 255, 100);

        vertexIndex++;
    }
}

}