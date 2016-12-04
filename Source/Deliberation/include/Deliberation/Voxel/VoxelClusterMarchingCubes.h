#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Voxel/VoxelCluster.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API VoxelClusterMarchingCubes final
{
public:
    VoxelClusterMarchingCubes(const VoxelCluster<glm::vec3> & cluster);

    void run();

    LayoutedBlob && takeVertices();

private:
    bool checkVoxel(i32 x, i32 y, i32 z) const;
    void generateMesh(i32 x, i32 y, i32 z, u8 configID);

private:
    const VoxelCluster<glm::vec3> & m_cluster;
    LayoutedBlob                    m_vertices;

    LayoutedBlob::Field<glm::vec3>  m_positions;
    LayoutedBlob::Field<glm::vec3>  m_normals;
    LayoutedBlob::Field<glm::vec3>  m_colors;
};

}