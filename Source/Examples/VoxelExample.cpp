#include <bitset>
#include <iostream>
#include <set>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "../../../Source/GameLib/Voxel/VoxelCluster.h"
#include "../../../Source/GameLib/Voxel/VoxelClusterMarchingCubes.h"
#include "../../../Source/GameLib/Voxel/VoxReader.h"
#include "../../../Source/GameLib/Voxel/VoxelClusterMarchingCubesTriangulation.h"
#include "../../../Source/GameLib/Voxel/VoxelClusterMarchingCubes.h"

#include "SceneExampleApplication.h"

using namespace deliberation;

class VoxelExample:
    public SceneExampleApplication
{
public:
    VoxelExample():
        SceneExampleApplication("VoxelExample")
    {

    }

    virtual void onStartup() override
    {
        SceneExampleApplication::onStartup();

        VoxelClusterMarchingCubesTriangulation marchingCubesTriangulation;

        m_ground.disengage();

        m_camera.setPosition({-15.0f, 1.0f, 15.0f});

        std::array<glm::vec3, 8> cornerColors = {{
                                                     {1.0f, 0.0f, 0.0f},
                                                     {1.0f, 1.0f, 0.0f},
                                                     {1.0f, 1.0f, 1.0f},
                                                     {0.0f, 1.0f, 1.0f},
                                                     {0.0f, 0.0f, 1.0f},
                                                     {1.0f, 0.0f, 1.0f},
                                                     {0.5f, 0.5f, 0.5f},
                                                     {1.0f, 0.0f, 0.5f}
                                                 }};

        auto & configs = marchingCubesTriangulation.configs();

        for (u32 configID = 0; configID < configs.size(); configID++)
        {
            if (configID != 158) continue;

            auto & triangulation = configs[configID];

            m_geometryRenderers.emplace_back(context(), m_camera);

            auto & renderer = m_geometryRenderers.back();

            auto centerX = i32(configID % 16) - 8;
            auto centerZ = -i32(configID / 16) + 8;

            auto center = glm::vec3{centerX, 1.0f, centerZ} * 2.4f;

            auto transform = Transform3D::atPosition(center);


            auto color = glm::vec3(1.0f, 1.0f, 1.0f);

            /**
             * Cube
             */
            auto & cube = renderer.addAndGetBox({0.5f, 0.5f, 0.5f}, color, true);
            cube.setTransform(transform);

            /**
             * Corners
             */
            std::bitset<8> configBits(configID);
            for (u32 b = 0; b < configBits.size(); b++)
            {
                if (!configBits.test(b))
                {
                    continue;
                }

                auto & sphere = renderer.addAndGetSphere(cornerColors[b], 0.1f);

                static std::array<glm::vec3, 8> corners = {{
                                                            {-0.5f, -0.5f, -0.5f},
                                                            {0.5f, -0.5f, -0.5f},
                                                            {0.5f,  -0.5f, 0.5f},
                                                            {-0.5f,  -0.5f, 0.5f},
                                                            {-0.5f, 0.5f, -0.5f},
                                                            {0.5f, 0.5f, -0.5f},
                                                            {0.5f,  0.5f, 0.5f},
                                                            {-0.5f,  0.5f, 0.5f},
                                                    }};

                auto p = corners[b];
                p.z *= -1.0f;
                auto cornerTransform = transform.worldTranslated(p);

                sphere.setTransform(cornerTransform);
            }

            /**
             * Triangles
             */
            auto & triangles = renderer.addAndGetWireframe();
            triangles.setTransform(transform);

            color = glm::vec3(1.0f, 0.2f, 0.2f);

            for (auto & triangle : triangulation)
            {
                auto pos = triangle.positions;

                pos[0].z *= -1.0f;
                pos[1].z *= -1.0f;
                pos[2].z *= -1.0f;

                triangles.addLineStrip({
                                           {pos[0], cornerColors[triangle.corner]},
                                           {pos[1], cornerColors[triangle.corner]},
                                           {pos[2], cornerColors[triangle.corner]},
                                           {pos[0], cornerColors[triangle.corner]},
                                       });
            }
        }

        /**
         *
         */
        VoxReader voxReader;

        auto clusters = voxReader.read(deliberation::DeliberationDataPath("Data/VoxelCluster/ship.vox"));
        if (!clusters.empty())
        {
            auto marchingCubes = VoxelClusterMarchingCubes(marchingCubesTriangulation, clusters[0]);
            marchingCubes.run();

            m_program = context().createProgram({deliberation::DeliberationDataPath("Data/Shaders/Voxel.vert"),
                                                 deliberation::DeliberationDataPath("Data/Shaders/Voxel.frag")});
            m_draw = context().createDraw(m_program);

            m_draw.addVertices(marchingCubes.takeVertices());
        }

        /**
         *
         */
        deliberation::DisableGLErrorChecks();
    }

    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);

        m_draw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_draw.uniform("Transform").set(m_transform.matrix());
        m_draw.schedule();

        for (auto & geometryRenderer : m_geometryRenderers)
        {
            geometryRenderer.schedule();
        }
    }

private:
    Program     m_program;
    Draw        m_draw;
    Transform3D m_transform;

    std::vector<DebugGeometryRenderer> m_geometryRenderers;
};


int main(int argc, char * argv[])
{


    return VoxelExample().run(argc, argv);
}

