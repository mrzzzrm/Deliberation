#include <bitset>
#include <iostream>
#include <set>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/Application.h>

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

        auto cluster = VoxelCluster<glm::vec3>({2,2,2});
        cluster.set({1, 0, 0}, {1.0f, 0.8f, 0.2f});
        cluster.set({1, 1, 0}, {1.0f, 0.8f, 0.2f});
        cluster.set({1, 0, 1}, {1.0f, 0.8f, 0.2f});
        cluster.set({0, 0, 1}, {1.0f, 0.8f, 0.2f});
        auto marchingCubes = VoxelClusterMarchingCubes(cluster);
        marchingCubes.run();

        m_program = context().createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
                                             deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
        m_draw = context().createDraw(m_program);

        m_draw.addVertices(marchingCubes.takeVertices());
      //  m_draw.state().setCullState(CullState::disabled());
    }

    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);

        m_draw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_draw.schedule();
    }

private:
    Program m_program;
    Draw    m_draw;
};


int main(int argc, char * argv[])
{


    return VoxelExample().run(argc, argv);
}

