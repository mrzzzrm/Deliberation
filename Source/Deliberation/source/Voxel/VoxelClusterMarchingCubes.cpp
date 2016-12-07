#include <Deliberation/Voxel/VoxelClusterMarchingCubes.h>

#include <bitset>
#include <iostream>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/IntTypes.h>

using namespace deliberation;

namespace
{

std::array<std::bitset<8>, 18> equivalenceClasses = {
    0b00000000, // 0
    0b00001000, // 1
    0b01001000, // 2
    0b00001001, // 3
    0b00101000, // 4
    0b00001110, // 5
    0b01001001, // 6
    0b01011000, // 7
    0b00011110, // 8
    0b01101001, // 9
    0b01011010, // 10
    0b00001111, // 11
    0b10001110, // 12
    0b01001101, // 13
    0b01001110, // 14
    0b11011110, // 15
    0b10011110, // 16
    0b01011110, // 17
};

std::array<std::vector<int>, 18> equivalenceClassMeshes = {
    std::vector<int>(), // 0
    std::vector<int>{{3,7,2}}, // 1
    std::vector<int>{{3,7,2, 10,9,6}}, // 2
    std::vector<int>{{0,4,7, 0,7,2}}, // 3
    std::vector<int>{{3,72, 8,5,9}}, // 4
    std::vector<int>{{0,3,5, 3,7,5, 5,7,6}}, // 5
    std::vector<int>{{3,7,2, 8,9,5, 9,10,6}}, // 6
    std::vector<int>{{3,7,2, 10,9,6, 11,4,8}}, // 7
    std::vector<int>{{0,3,5, 3,7,5, 5,7,6, 11,4,8}}, // 8
    std::vector<int>{{0,4,7, 0,7,2, 5,10,8, 5,6,10}}, // 9
    std::vector<int>{{3,7,2, 10,9,6, 11,4,8, 0,1,5}}, // 10
    std::vector<int>{{4,7,5, 5,7,6}}, // 11
    std::vector<int>{{3,11,0, 0,11,5, 5,11,10, 5,10,6}}, // 12
    std::vector<int>{{10,4,7, 4,10,9, 4,9,0, 0,9,1}}, // 13
    std::vector<int>{{3,7,10, 3,10,9, 3,9,0, 0,9,5}}, // 14
    std::vector<int>{{4,8,3, 3,8,9, 3,9,5, 3,5,0}}, // 15
    std::vector<int>{{4,8,3, 8,10,3, 3,10,6, 3,6,5, 3,5,0}}, // 16
    std::vector<int>{{3,7,10, 3,10,9, 3,9,0, 0,9,5, 4,8,11}}, // 17
};

std::array<glm::vec3, 12> edgePositions = {{
                                               {0.0f, -0.5f, -0.5f},
                                               {0.5f, -0.5f, 0.0f},
                                               {0.0f, -0.5f, 0.5f},
                                               {-0.5f, -0.5f, 0.0f},
                                               {-0.5f, 0.0f, -0.5f},
                                               {0.5f, 0.0f, -0.5f},
                                               {0.5f, 0.0f, 0.5f},
                                               {-0.5f, 0.0f, 0.5f},
                                               {0.0f, 0.5f, -0.5f},
                                               {0.5f, 0.5f, 0.0f},
                                               {0.0f, 0.5f, 0.5f},
                                               {-0.5f, 0.5f, 0.0f},
                                           }};


struct Config {
    u8 equivalenceClass = 255;
    glm::u8vec3 rotation;
    bool inverse;
    bool set = false;
};

std::bitset<8> rotateX(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[4]);
    result.set(1, config[5]);
    result.set(2, config[1]);
    result.set(3, config[0]);
    result.set(4, config[7]);
    result.set(5, config[6]);
    result.set(6, config[2]);
    result.set(7, config[3]);
    return result;
}

std::bitset<8> rotateY(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[3]);
    result.set(1, config[0]);
    result.set(2, config[1]);
    result.set(3, config[2]);
    result.set(4, config[7]);
    result.set(5, config[4]);
    result.set(6, config[5]);
    result.set(7, config[6]);
    return result;
}

std::bitset<8> rotateZ(const std::bitset<8> & config) {
    std::bitset<8> result;
    result.set(0, config[1]);
    result.set(1, config[5]);
    result.set(2, config[6]);
    result.set(3, config[2]);
    result.set(4, config[0]);
    result.set(5, config[4]);
    result.set(6, config[7]);
    result.set(7, config[3]);
    return result;
}

struct Configs {
    static Configs & get() {
        static Configs instance;
        return instance;
    }

    Configs() {
        std::array<Config, 256> configs;

        for (size_t e = 0; e < equivalenceClasses.size(); e++) {
            auto & equivalenceClass = equivalenceClasses[e];

            std::bitset<8> baseClassX = equivalenceClass;
            for (auto x = 0; x < 4; x++) {
                std::bitset<8> baseClassY = baseClassX;
                for (auto y = 0; y < 4; y++) {
                    std::bitset<8> baseClassZ = baseClassY;
                    for (auto z = 0; z < 4; z++) {
                        auto configID = baseClassZ.to_ullong();

                        if (!configs[configID].set) {
                            configs[configID].set = true;
                            configs[configID].equivalenceClass = e;
                            configs[configID].rotation = {x, y, z};
                            configs[configID].inverse = false;
                        }

                        auto flipped = baseClassZ;
                        flipped.flip();

                        configID = flipped.to_ullong();

                        if (!configs[configID].set) {
                            configs[configID].set = true;
                            configs[configID].equivalenceClass = e;
                            configs[configID].rotation = {x, y, z};
                            configs[configID].inverse = true;
                        }

                        baseClassZ = rotateZ(baseClassZ);
                    }

                    baseClassY = rotateY(baseClassY);
                }

                baseClassX = rotateX(baseClassX);
            }
        }

        for (auto configID = 0; configID < 256; configID++)
        {
            auto & config = configs[configID];
            std::cout << configID << " = "<< (int)config.equivalenceClass << " with {" << (int)config.rotation.x << " " << (int)config.rotation.y << " " << (int)config.rotation.z << "}" << std::endl;
        }

        for (size_t c = 0; c < configs.size(); c++) {
            auto & config = configs[c];

            auto & edgeIndices = equivalenceClassMeshes[config.equivalenceClass];

            for (auto & index : edgeIndices) {
                auto position = edgePositions[index];

                for (auto i = 0; i < config.rotation.x; i++)
                {
                    position = {position.x, position.z, -position.y};
                }

                for (auto i = 0; i < config.rotation.y; i++)
                {
                    position = {-position.z, position.y, position.x};
                }

                for (auto i = 0; i < config.rotation.z; i++)
                {
                    position = {position.y, -position.x, position.z};
                }

                positions[c].emplace_back(position);
            }

            for (size_t v = 0; v < positions[c].size(); v += 3)
            {
                auto a = positions[c][v] - positions[c][v + 2];
                auto b = positions[c][v + 1] - positions[c][v + 2];
                normals[c].emplace_back(glm::normalize(glm::cross(a, b)));
            }
        }
    }

    std::array<std::vector<glm::vec3>, 256> positions;
    std::array<std::vector<glm::vec3>, 256> normals;
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
//                if (x != 0 || y != 1 || z != 1)
//                {
//                    continue;
//                }

                config.reset();

                config.set(0, checkVoxel(x - 1, y - 1, z - 1));
                config.set(1, checkVoxel(x - 0, y - 1, z - 1));
                config.set(2, checkVoxel(x - 0, y - 1, z - 0));
                config.set(3, checkVoxel(x - 1, y - 1, z - 0));
                config.set(4, checkVoxel(x - 1, y - 0, z - 1));
                config.set(5, checkVoxel(x - 0, y - 0, z - 1));
                config.set(6, checkVoxel(x - 0, y - 0, z - 0));
                config.set(7, checkVoxel(x - 1, y - 0, z - 0));

                if (config.to_ullong() != 0)
                {
                    std::cout << x << " " << y << " " << z << " -> " << config.to_ullong() << std::endl;
                }

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
    auto &positions = Configs::get().positions[configID];
    auto &normals = Configs::get().normals[configID];

    if (positions.empty())
    {
        return;
    }

    auto vertexIndex = m_vertices.count();

    m_vertices.resize(m_vertices.count() + positions.size());

    for (size_t p = 0; p < positions.size(); p++)
    {
        (*m_positions)[vertexIndex] = positions[p] + glm::vec3(x, y, z);
        (*m_normals)[vertexIndex] = normals[p/3];
        (*m_colors)[vertexIndex] = glm::vec3(100, 255, 100);

        vertexIndex++;
    }
}

}