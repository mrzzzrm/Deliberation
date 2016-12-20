#pragma once

#include <bitset>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/Vertices.h>

#include <Deliberation/Voxel/VoxelCluster.h>
#include <Deliberation/Voxel/VoxelClusterMarchingCubesTriangulation.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API VoxelClusterMarchingCubes2 final
{
public:
    VoxelClusterMarchingCubes2(const VoxelClusterMarchingCubesTriangulation & triangulation,
                               const VoxelCluster<glm::vec3> & cluster);

    void run();

    LayoutedBlob && takeVertices();

private:
    bool checkVoxel(i32 x, i32 y, i32 z) const;
    void generateMesh(i32 x, i32 y, i32 z, u8 configID);
    glm::vec3 getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const;

private:
    const VoxelClusterMarchingCubesTriangulation &  m_triangulation;
    const VoxelCluster<glm::vec3> &                 m_cluster;

    LayoutedBlob                                    m_vertices;

    Optional<LayoutedBlob::Field<glm::vec3>>        m_positions;
    Optional<LayoutedBlob::Field<glm::vec3>>        m_normals;
    Optional<LayoutedBlob::Field<glm::vec3>>        m_colors;
};

}