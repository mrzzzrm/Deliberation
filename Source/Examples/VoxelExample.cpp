#include <bitset>
#include <iostream>
#include <set>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/DebugGeometryRenderer.h>

#include <Deliberation/Voxel/VoxelCluster.h>
#include <Deliberation/Voxel/VoxelClusterMarchingCubes.h>
#include <Deliberation/Voxel/VoxReader.h>

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

        m_ground.disengage();

        m_camera.setPosition({-15.0f, 1.0f, 15.0f});

        auto cluster = VoxelCluster<glm::vec3>({3,2,7});
        cluster.set({0, 0, 0}, {1.0f, 0.8f, 0.2f});
        cluster.set({0, 1, 0}, {0.4f, 0.4f, 0.4f});
        cluster.set({0, 2, 0}, {0.4f, 0.7f, 0.7f});
        cluster.set({0, 3, 0}, {0.4f, 0.7f, 0.7f});
        cluster.set({1, 0, 1}, {0.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 1}, {1.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 2}, {1.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 3}, {0.8f, 0.9f, 0.2f});
        auto marchingCubes = VoxelClusterMarchingCubes(cluster);
        marchingCubes.run();

//        m_program = context().createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
//                                             deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
//        m_draw = context().createDraw(m_program);

//        m_draw.addVertices(marchingCubes.takeVertices());

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

        for (u32 config = 0; config < 256; config++)
        {
            m_geometryRenderers.emplace_back(context(), m_camera);

            auto & renderer = m_geometryRenderers.back();

            auto centerX = i32(config % 16) - 8;
            auto centerZ = -i32(config / 16) + 8;

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
            std::bitset<8> configBits(config);
            for (u32 b = 0; b < configBits.size(); b++)
            {
                if (!configBits.test(b))
                {
                    continue;
                }

                auto & sphere = renderer.addAndGetSphere(cornerColors[b], 0.1f);

                auto cornerTransform = transform.worldTranslated(marchingCubes.cornerOffset(b));

                sphere.setTransform(cornerTransform);
            }

            /**
             * Triangles
             */
            auto & triangles = renderer.addAndGetWireframe();
            triangles.setTransform(transform);

            color = glm::vec3(1.0f, 0.2f, 0.2f);

            Assert(marchingCubes.configNumVertices(config) % 3 == 0, "");
            for (u32 v = 0; v < marchingCubes.configNumVertices(config); v += 3)
            {
                triangles.addLineStrip({
                                           {marchingCubes.configVertex(config, v).position, cornerColors[marchingCubes.configTriangleCorner(config, v/3)]},
                                           {marchingCubes.configVertex(config, v + 1).position, cornerColors[marchingCubes.configTriangleCorner(config, v/3)]},
                                           {marchingCubes.configVertex(config, v + 2).position, cornerColors[marchingCubes.configTriangleCorner(config, v/3)]},
                                           {marchingCubes.configVertex(config, v).position, cornerColors[marchingCubes.configTriangleCorner(config, v/3)]},
                                       });
            }
        }

        /**
         *
         */
        VoxReader voxReader;

        auto clusters = voxReader.read(deliberation::dataPath("Data/VoxelCluster/station.vox"));
        if (!clusters.empty())
        {
            auto marchingCubes = VoxelClusterMarchingCubes(clusters[0]);
            marchingCubes.run();

            m_program = context().createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
                                                 deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
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

