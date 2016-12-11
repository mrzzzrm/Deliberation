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

        auto cluster = VoxelCluster<glm::vec3>({3,2,7});
        cluster.set({0, 0, 0}, {1.0f, 0.8f, 0.2f});
        cluster.set({1, 0, 1}, {0.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 1}, {1.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 2}, {1.0f, 0.8f, 0.2f});
        auto marchingCubes = VoxelClusterMarchingCubes(cluster);
        marchingCubes.run();

        m_program = context().createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
                                             deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
        m_draw = context().createDraw(m_program);

        m_draw.addVertices(marchingCubes.takeVertices());
      //  m_draw.state().rasterizerState().setPrimitive(gl::GL_LINE_LOOP);
      //  m_draw.state().setCullState(CullState::disabled());

        std::array<glm::vec3, 8> cornerColors = {
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 0.0f, 0.5f}
        };

        for (u8 config = 0; config < 256; config++)
        {
            m_geometryRenderers.emplace_back(context(), m_camera);

            auto & renderer = m_geometryRenderers.back();

            auto centerX = (config % 16) - 8;
            auto centerZ = -(config / 16) + 8;

            auto transform = Transform3D::atPosition({centerX, 1.0f, centerZ});


            auto color = glm::vec3(1.0f, 1.0f, 1.0f);

            /**
             * Cube
             */
            auto & cube = renderer.addAndGetBox({0.5f, 0.5f, 0.5f}, color);
            cube.setTransform(transform);
//            cube.addLineStrip({
//                                       {{-0.5f, -0.5f, 0.5f}, color},
//                                       {{ 0.5f, -0.5f, 0.5f}, color},
//                                       {{ 0.5f,  0.5f, 0.5f}, color},
//                                       {{-0.5f,  0.5f, 0.5f}, color}
//                                   });
//            cube.addLineStrip({
//                                       {{-0.5f, -0.5f,-0.5f}, color},
//                                       {{ 0.5f, -0.5f,-0.5f}, color},
//                                       {{ 0.5f,  0.5f,-0.5f}, color},
//                                       {{-0.5f,  0.5f,-0.5f}, color}
//                                   });
//
//            cube.addLineStrip({{{-0.5f, -0.5f, -0.5f}, color}, {{-0.5f, -0.5f, 0.5f}, color}});
//            cube.addLineStrip({{{ 0.5f, -0.5f, -0.5f}, color}, {{ 0.5f, -0.5f, 0.5f}, color}});
//            cube.addLineStrip({{{ 0.5f,  0.5f, -0.5f}, color}, {{ 0.5f,  0.5f, 0.5f}, color}});
//            cube.addLineStrip({{{-0.5f,  0.5f, -0.5f}, color}, {{-0.5f,  0.5f, 0.5f}, color}});

            /**
             * Corners
             */
            std::bitset<8> configBits;
            for (u32 b = 0; b < configBits.size(); b++)
            {
                auto & sphere = renderer.addAndGetSphere({0.3f, 0.3f, 1.0f}, 0.1f);
                sphere.setTransform(Transform3D::atPosition(transform.worldTranslated(marchingCubes.cornerOffset(b))));
            }

            /**
             * Triangles
             */
            auto & triangles = renderer.addAndGetWireframe();

            color = glm::vec3(1.0f, 0.2f, 0.2f);

            Assert(marchingCubes.configNumVertices(config) % 3 == 0, "");
            for (u32 v = 0; v < marchingCubes.configNumVertices(config); v += 3)
            {
                triangles.addLineStrip({
                                           {marchingCubes.configVertex(config, v).position, cornerColors[marchingCubes.configTriangleCorner(v/3)]},
                                           {marchingCubes.configVertex(config, v + 1).position, cornerColors[marchingCubes.configTriangleCorner(v/3)]},
                                           {marchingCubes.configVertex(config, v + 2).position, cornerColors[marchingCubes.configTriangleCorner(v/3)]},
                                           {marchingCubes.configVertex(config, v).position, cornerColors[marchingCubes.configTriangleCorner(v/3)]},
                                       });
            }
        }
    }

    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);

        m_draw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_draw.schedule();

        for (auto & geometryRenderer : m_geometryRenderers)
        {
            geometryRenderer.schedule();
        }
    }

private:
    Program m_program;
    Draw    m_draw;

    std::vector<DebugGeometryRenderer> m_geometryRenderers;
};


int main(int argc, char * argv[])
{


    return VoxelExample().run(argc, argv);
}

