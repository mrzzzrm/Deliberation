#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Mesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/Debug/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

using namespace deliberation;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class BasicMeshExample:
    public deliberation::Application
{
public:
    BasicMeshExample():
        deliberation::Application("BasicMeshExample")
    {

    }

    virtual void onStartup() override
    {
        m_grid.reset(context(), 0.5f, m_camera);

        m_camera.setPosition({0.0f, 1.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(640.0f/480.0f);

        m_draw = createDraw();
        m_clear = context().createClear();

        m_viewProjectionHandle = m_draw.uniform("ViewProjection");
        m_transformHandle = m_draw.uniform("Transform");

        m_navigator.reset(m_camera, input(), 1.0f);
    }

    deliberation::Draw createDraw()
    {
        deliberation::DataLayout layout({{"Position", deliberation::Type_Vec3},
                                         {"Color", deliberation::Type_Vec3}});
        deliberation::LayoutedBlob meshVertices(layout, 5);

        auto positions = meshVertices.field<glm::vec3>("Position");
        auto colors = meshVertices.field<glm::vec3>("Color");

        positions[0] = glm::vec3(-1, 0, 1);
        colors[0]    = glm::vec3(1, 0, 0);

        positions[1] = glm::vec3(-1, 0, -1);
        colors[1]    = glm::vec3(1, 0, 0);

        positions[2] = glm::vec3(1, 0, -1);
        colors[2]    = glm::vec3(1, 0, 0);

        positions[3] = glm::vec3(1, 0, 1);
        colors[3]    = glm::vec3(1, 0, 0);

        positions[4] = glm::vec3(0, 1, 0);
        colors[4]    = glm::vec3(1, 1, 1);

        deliberation::LayoutedBlob faceAttributes;

        deliberation::Mesh::Faces faces(5);
        faces[0] = {{0, 3, 4}};
        faces[1] = {{3, 2, 4}};
        faces[2] = {{1, 4, 2}};
        faces[3] = {{0, 4, 1}};
        faces[4] = {{0, 1, 2, 3}};

        deliberation::Mesh mesh(std::move(meshVertices), std::move(faceAttributes), std::move(faces));

        deliberation::MeshCompiler compiler;
        auto compiledMesh = compiler.compile(mesh);

        auto program = context().createProgram({deliberation::dataPath("Data/BasicMeshTest.vert"),
                                                deliberation::dataPath("Data/BasicMeshTest.frag")});

        auto cpositions = compiledMesh.vertices.field<glm::vec3>("Position");
        std::cout << "Vertices: " << std::endl;
        for (int i = 0; i < compiledMesh.vertices.count(); i++)
        {
            std::cout << "  [" << i << "] = " << cpositions[i] << std::endl;
        }

        auto draw = context().createDraw(program, gl::GL_TRIANGLES);
        draw.addVertices(compiledMesh.vertices.layout(), compiledMesh.vertices.rawData());
        draw.setIndices32(compiledMesh.indices.rawData());

        return draw;
    }

    virtual void onFrame(float seconds) override
    {
        m_navigator.get().update(seconds);

        m_transform.worldRotate(glm::quat({glm::pi<float>() * 0.0f * seconds,
                                           glm::pi<float>() * 0.2f * seconds,
                                           glm::pi<float>() * 0.0f * seconds}));

        m_viewProjectionHandle.set(m_camera.viewProjection());
        m_transformHandle.set(m_transform.matrix());

        m_clear.schedule();
        m_draw.schedule();
        m_grid.get().draw();
    }

private:
    deliberation::Draw                  m_draw;
    deliberation::Clear                 m_clear;
    deliberation::Optional<deliberation::DebugGrid3DRenderer>
                                        m_grid;
    deliberation::Camera3D              m_camera;
    deliberation::Transform3D           m_transform;
    deliberation::Texture               m_depthBuffer;
    deliberation::Texture               m_colorBuffer;
    deliberation::Uniform               m_viewProjectionHandle;
    deliberation::Uniform               m_transformHandle;
    deliberation::Optional<deliberation::DebugCameraNavigator3D>
                                        m_navigator;

};

int main(int argc, char * argv[])
{
    return BasicMeshExample().run();
}

