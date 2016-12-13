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

std::array<std::vector<int>, 18> rawEquivalenceClassMeshes = {
    std::vector<int>(), // 0
    std::vector<int>{{3,7,2}}, // 1
    std::vector<int>{{3,7,2, 10,9,6}}, // 2
    std::vector<int>{{0,4,7, 0,7,2}}, // 3
    std::vector<int>{{3,7,2, 8,5,9}}, // 4
    std::vector<int>{{0,3,5, 3,7,5, 5,7,6}}, // 5
    std::vector<int>{{0,4,7, 0,7,2, 10,9,6}}, // 6
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

std::array<glm::vec3, 8> corners = {{
                                        {-0.5f, -0.5f, -0.5f},
                                        {0.5f, -0.5f, -0.5f},
                                        {0.5f,  -0.5f, 0.5f},
                                        {-0.5f,  -0.5f, 0.5f},
                                        {-0.5f, 0.5f, -0.5f},
                                        {0.5f, 0.5f, -0.5f},
                                        {0.5f,  0.5f, 0.5f},
                                        {-0.5f,  0.5f, 0.5f},
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
        /**
         * Generate inverted configs
         */
        std::copy(equivalenceClasses.begin(), equivalenceClasses.end(), m_equivalenceClasses.begin());
        std::copy(rawEquivalenceClassMeshes.begin(), rawEquivalenceClassMeshes.end(), m_rawEquivalenceClassMeshes.begin());

        for (size_t e = 0; e < equivalenceClasses.size(); e++)
        {
            auto & equivalenceClass = equivalenceClasses[e];
            auto flippedEquivalenceClass = equivalenceClass;
            flippedEquivalenceClass.flip();
            m_equivalenceClasses[e + 16] = flippedEquivalenceClass;
        }

        for (size_t m = 0; m < rawEquivalenceClassMeshes.size(); m++)
        {
            auto & mesh = rawEquivalenceClassMeshes[m];
            auto flippedMesh = mesh;

            Assert(flippedMesh.size() % 3 == 0, "Vertices don't form triangles");
            for (size_t i = 0; i < flippedMesh.size(); i += 3)
            {
                std::swap(flippedMesh[i + 1], flippedMesh[i + 2]);
            }

            m_rawEquivalenceClassMeshes[m + 16] = std::move(flippedMesh);
        }


        /**
         * Re-triangulate the raw equivalence class meshes to obtain triangles assigned to one corner
         */
        for (size_t e = 0; e < m_rawEquivalenceClassMeshes.size(); e++)
        {
            auto & rawEquivalenceClassMesh = m_rawEquivalenceClassMeshes[e];

            Assert(rawEquivalenceClassMesh.size() % 3 == 0, "No triangles in raw mesh")
            for (size_t inV = 0; inV < rawEquivalenceClassMesh.size(); inV += 3)
            {
                std::array<u8, 3> vertexCorners;

                for (size_t v = 0; v < 3; v++)
                {
                    vertexCorners[v] = getCornerOfEdgeIndex(m_equivalenceClasses[e], rawEquivalenceClassMesh[inV + v]);
                }

                auto & a = edgePositions[rawEquivalenceClassMesh[inV + 0]];
                auto & b = edgePositions[rawEquivalenceClassMesh[inV + 1]];
                auto & c = edgePositions[rawEquivalenceClassMesh[inV + 2]];

                /**
                 * All vertices share the same corner, keep triangle
                 */
                if (vertexCorners[0] == vertexCorners[1] && vertexCorners[1] == vertexCorners[2])
                {
                    addTriangleToEquivalenceClass(e, a, b, c, vertexCorners[0]);
                    continue;
                }

                /**
                 * Two different corners share triangle
                 */
                {
                    auto bisectTriangle = [&e, this] (const glm::vec3 & a0, const glm::vec3 & a1, const glm::vec3 & b, u8 c0, u8 c1)
                    {
                        auto j = (a0 + b) / 2.0f;
                        auto k = (a1 + b) / 2.0f;

                        addTriangleToEquivalenceClass(e, a0, a1, j, c0);
                        addTriangleToEquivalenceClass(e, j, a1, k, c0);
                        addTriangleToEquivalenceClass(e, j, k, b, c1);

                    };

                    if (vertexCorners[0] == vertexCorners[1])
                    {
                        bisectTriangle(a, b, c, vertexCorners[0], vertexCorners[2]);
                        continue;
                    }
                    if (vertexCorners[0] == vertexCorners[2])
                    {
                        bisectTriangle(c, a, b, vertexCorners[0], vertexCorners[1]);
                        continue;
                    }
                    if (vertexCorners[1] == vertexCorners[2])
                    {
                        bisectTriangle(b, c, a, vertexCorners[1], vertexCorners[0]);
                        continue;
                    }
                }

                /**
                 * All three vertices belong to different corners
                 */
                auto j = (a + b) / 2.0f;
                auto k = (b + c) / 2.0f;
                auto l = (c + a) / 2.0f;
                auto m = (a + b + c) / 3.0f;

                addTriangleToEquivalenceClass(e, a, m, l, vertexCorners[0]);
                addTriangleToEquivalenceClass(e, a, j, m, vertexCorners[0]);
                addTriangleToEquivalenceClass(e, b, m, j, vertexCorners[1]);
                addTriangleToEquivalenceClass(e, b, k, m, vertexCorners[1]);
                addTriangleToEquivalenceClass(e, c, m, k, vertexCorners[2]);
                addTriangleToEquivalenceClass(e, c, l, m, vertexCorners[2]);
            }
        }


        /**
         *
         */
        for (size_t e = 0; e < m_equivalenceClasses.size(); e++) {
            auto & equivalenceClass = m_equivalenceClasses[e];

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

            auto & unrotatedVertices = equivalenceClassMeshes[config.equivalenceClass];
            auto & unrotatedCorners = equivalenceClassCorners[config.equivalenceClass];

            positions[c].resize(unrotatedVertices.size());
            corners[c].resize(unrotatedCorners.size());

            /**
             * Rotate vertices into position
             */
            for (size_t i = 0; i < unrotatedVertices.size(); i++) {
                auto position = unrotatedVertices[i];

                for (auto r = 0; r < config.rotation.x; r++)
                {
                    position = {position.x, position.z, -position.y};
                }

                for (auto r = 0; r < config.rotation.y; r++)
                {
                    position = {-position.z, position.y, position.x};
                }

                for (auto r = 0; r < config.rotation.z; r++)
                {
                    position = {position.y, -position.x, position.z};
                }

                positions[c][i] = position;
            }

            /**
             * Rotate corners into position
             */
            for (size_t i = 0; i < unrotatedCorners.size(); i++) {
                auto corner = unrotatedCorners[i];

                static u8 x[] = {3, 2, 6, 7, 0, 1, 5, 4};
                static u8 y[] = {1, 2, 3, 0, 5, 6, 7, 4};
                static u8 z[] = {4, 0, 3, 7, 5, 1, 2, 6};

                for (auto r = 0; r < config.rotation.x; r++)
                {
                    corner = x[corner];
                }

                for (auto r = 0; r < config.rotation.y; r++)
                {
                    corner = y[corner];
                }

                for (auto r = 0; r < config.rotation.z; r++)
                {
                    corner = z[corner];
                }

                corners[c][i] = corner;
            }

            /**
             * Generate normals
             */
            Assert(positions[c].size() % 3 == 0, "Vertices don't form triangles");
            for (size_t v = 0; v < positions[c].size(); v += 3)
            {
                auto a = positions[c][v] - positions[c][v + 2];
                auto b = positions[c][v + 1] - positions[c][v + 2];
                normals[c].emplace_back(glm::normalize(glm::cross(a, b)));
            }
        }
    }


    void addTriangleToEquivalenceClass(u8 e,
                                       const glm::vec3 & a,
                                       const glm::vec3 & b,
                                       const glm::vec3 & c,
                                       u8 corner)
    {
        equivalenceClassMeshes[e].emplace_back(a);
        equivalenceClassMeshes[e].emplace_back(b);
        equivalenceClassMeshes[e].emplace_back(c);
        equivalenceClassCorners[e].emplace_back(corner);
    }

    u8 getCornerOfEdgeIndex(const std::bitset<8> & config, u8 edgeIndex) const
    {
        Assert(edgeIndex < 12, "Invalid edge index");

        u8 a;
        u8 b;

        if (edgeIndex < 4)
        {
            a = edgeIndex;
            b = (edgeIndex + 1) % 4;
        }
        else if (edgeIndex < 8)
        {
            a = edgeIndex - 4;
            b = edgeIndex;
        }
        else
        {
            a = edgeIndex - 4;
            b = (edgeIndex - 4 + 1) % 4 + 4;
        }

        Assert(config[a] ^ config[b], "");

        return (u8) (config[a] ? a : b);
    }

    std::array<std::bitset<8>, 36>          m_equivalenceClasses;
    std::array<std::vector<int>, 36>        m_rawEquivalenceClassMeshes;

    std::array<std::vector<glm::vec3>, 36>  equivalenceClassMeshes;
    std::array<std::vector<u8>, 36>         equivalenceClassCorners;

    std::array<Config, 256>                 configs;

    std::array<std::vector<glm::vec3>, 256> positions;
    std::array<std::vector<u8>, 256>        corners;
    std::array<std::vector<glm::vec3>, 256> normals;
};

}

namespace deliberation
{

VoxelClusterMarchingCubes::VoxelClusterMarchingCubes(const VoxelCluster<glm::vec3> &cluster) :
    m_cluster(cluster)
{
    std::cout << rotateY(std::bitset<8>("01001101")) << std::endl;
    std::cout << rotateZ(rotateY(std::bitset<8>("01001101"))).to_ullong() << std::endl;
}


u32 VoxelClusterMarchingCubes::configNumVertices(u8 config) const
{
    return Configs::get().positions[config].size();
}

NormalVertex VoxelClusterMarchingCubes::configVertex(u8 config, u32 vertex) const
{
    Assert(vertex < Configs::get().positions[config].size(), "");
    Assert(vertex / 3 < Configs::get().normals[config].size(), "");

    NormalVertex result;
    result.position = Configs::get().positions[config][vertex];
    result.normal = Configs::get().normals[config][vertex / 3];

    return result;
}

u8 VoxelClusterMarchingCubes::configTriangleCorner(u8 config, u32 triangle) const
{
    Assert(triangle < Configs::get().corners[config].size(), "");
    return Configs::get().corners[config][triangle];
}

const glm::vec3 & VoxelClusterMarchingCubes::cornerOffset(u8 corner) const
{
    Assert(corner < 8, "Not a corner index");
    return corners[corner];
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
    auto &positions = Configs::get().positions[configID];
    auto &corners = Configs::get().corners[configID];
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
        (*m_colors)[vertexIndex] = getCubeColorAtCorner(x, y, z, corners[p/3]);

        vertexIndex++;
    }
}

glm::vec3 VoxelClusterMarchingCubes::getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const
{
    Assert(corner < 8, "Illegal corner index");

    static std::array<glm::ivec3, 8> cornerOffsets = {{
                                                         {-1, -1, -1},
                                                         {0, -1, -1},
                                                         {0, -1, 0},
                                                         {-1, -1, 0},
                                                         {-1, 0, -1},
                                                         {0, 0, -1},
                                                         {0, 0, 0},
                                                         {-1, 0, 0},
                                                     }};

    auto voxel = cornerOffsets[corner] + glm::ivec3(x, y, z);

    Assert(voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0, "");

    return m_cluster.get(voxel);
}

}