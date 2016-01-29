#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/Transform3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>

deliberation::Camera3D camera;
deliberation::Transform3D transform;
deliberation::Texture depthBuffer;
deliberation::Texture colorBuffer;
GLFWwindow * window;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

deliberation::Draw createDraw(deliberation::Context & context)
{
    auto program = context.createProgram({
       deliberation::dataPath("Data/Examples/BasicSceneExample.vert"),
       deliberation::dataPath("Data/Examples/BasicSceneExample.frag")
    });

    deliberation::UVSphere sphere(7, 7);
    auto mesh = sphere.generate();

    deliberation::MeshCompiler<deliberation::UVSphere::Vertex> compiler(mesh);
    compiler.compile();

    auto layout = context.createBufferLayout<deliberation::UVSphere::Vertex>({
        {"Position", &deliberation::UVSphere::Vertex::position},
        {"Normal", &deliberation::UVSphere::Vertex::normal}
    });

    auto draw = context.createDraw(program, gl::GL_TRIANGLES);
    draw.addVertices(layout, compiler.vertices());
    draw.setIndices32(compiler.indices());

    return draw;
}

int main(int argc, char * argv[])
{
    std::cout << "---- BasicSceneExample ----" << std::endl;

    // Init GLFW
    {
        if (!glfwInit())
        {
            return -1;
        }
        window = glfwCreateWindow(640, 480, "BasicSceneExample", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
    }

    glbinding::Binding::initialize();

    deliberation::init();
    deliberation::setPrefixPath("..");

    deliberation::Context context;

    deliberation::DebugGrid3DRenderer grid(context, 0.5f, camera);

    camera.setPosition({0.0f, 1.0f, 3.0f});
    camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
    camera.setAspectRatio(640.0f/480.0f);

    auto draw = createDraw(context);
    auto clear = context.createClear();

    auto viewProjectionHandle = draw.uniform("ViewProjection");
    auto transformHandle = draw.uniform("Transform");

    auto lastFrame = std::chrono::system_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        auto now = std::chrono::system_clock::now();
        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastFrame);
        lastFrame = now;

        transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * seconds.count(), glm::pi<float>() * 0.2f * seconds.count(), glm::pi<float>() * 0.1f * seconds.count()}));

        viewProjectionHandle.set(camera.viewProjection());
        transformHandle.set(transform.matrix());

        clear.schedule();
        draw.schedule();
        grid.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deliberation::shutdown();

    glfwTerminate();

    return 0;
}

