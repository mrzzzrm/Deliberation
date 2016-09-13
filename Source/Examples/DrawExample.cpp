#include <iostream>
#include <stdlib.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>

using namespace deliberation;

class UniformBufferExample:
    public Application
{
public:
    UniformBufferExample():
        Application("UniformBufferExample")
    {

    }

    virtual void onStartup() override
    {
        std::cout << "------------------------ UniformBufferExample ------------------------" << std::endl;

        m_program = context().createProgram({"Data/Examples/UniformBufferExample.vert",
                                             "Data/Examples/UniformBufferExample.frag"});
        m_globalUniforms = context().createBuffer(m_program.interface().uniformBlock("GlobalBlock").layout());
        m_localUniforms = context().createBuffer(m_program.interface().uniformBlock("LocalBlock").layout());

        m_globals = LayoutedBlob(m_globalUniforms.layout(), 1);
        m_locals = LayoutedBlob(m_localUniforms.layout(), 1);

        CuboidMesh mesh({1.0f, 2.0f, 1.8f});

        auto compilation = MeshCompiler().compile(mesh.generate(), Primitive_Lines);

        m_draw = context().createDraw(m_program, gl::GL_LINES);
        m_draw.addVertices(compilation.vertices);
        m_draw.setIndices(compilation.indices);
        m_draw.setUniformBuffer("GlobalBlock", m_globalUniforms);
        m_draw.setUniformBuffer("LocalBlock", m_localUniforms);
    }

    virtual void onFrame(float seconds) override
    {
        m_globals.field<glm::mat4>("ViewProjection")[0] = m_camera.viewProjection();
        m_globals.field<float>("Time")[0] = m_time;

        m_locals.field<glm::mat4>("Transform")[0] = m_transform.matrix();
        m_locals.field<uint32_t>("Type")[0] = 1u;

        m_globalUniforms.scheduleUpload(m_globals);
        m_localUniforms.scheduleUpload(m_locals);

        m_draw.schedule();

        m_time += seconds;
        quit(0);
    }

private:
    Camera3D        m_camera;
    Transform3D     m_transform;
    Program         m_program;
    Buffer          m_globalUniforms;
    Buffer          m_localUniforms;
    LayoutedBlob    m_globals;
    LayoutedBlob    m_locals;
    Draw            m_draw;
    float           m_time = 0.0f;
};

int main()
{
    UniformBufferExample().run();

    return 0;
}