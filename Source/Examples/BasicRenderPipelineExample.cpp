#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LapTimer.h>
#include <Deliberation/Core/Viewport.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Scene/Blit.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/RenderNode.h>
#include <Deliberation/Scene/RenderPipeline.h>
#include <Deliberation/Scene/MeshCompiler.h>

GLFWwindow * window;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

deliberation::Draw createSphereDraw(deliberation::Context & context, deliberation::Framebuffer & framebuffer)
{
    auto program = context.createProgram({
       deliberation::dataPath("Data/Examples/BasicRenderPipelineExample.vert"),
       deliberation::dataPath("Data/Examples/BasicRenderPipelineExample.frag")
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

    draw.setFramebuffer(framebuffer);

    return draw;
}

int main(int argc, char * argv[])
{
    std::cout << "---- BasicRenderPipelineExample ----" << std::endl;

    // Init GLFW
    {
        if (!glfwInit())
        {
            return -1;
        }
        window = glfwCreateWindow(1600, 900, "BasicRenderTargetExample", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
    }

    glbinding::Binding::initialize();

    deliberation::init();
    deliberation::setPrefixPath("/home/moritz/Coding/VoxelAdvent/Extern/Deliberation/");

    deliberation::Context context;

    deliberation::Camera3D camera;
    camera.setPosition({0.0f, 1.0f, 3.0f});
    camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
    camera.setAspectRatio(800.0f/450.0f);

    deliberation::Transform3D transform;

    deliberation::LapTimer timer;

    deliberation::Framebuffer fb = context.createFramebuffer(800, 450);
    fb.addDepthTarget(deliberation::PixelFormat_Depth_32F);
    fb.addRenderTarget(deliberation::PixelFormat_RGB_32F);
    fb.addRenderTarget(deliberation::PixelFormat_RGBA_32F);

    auto bbClear = context.createClear();
    auto fbClear = fb.createClear();
    auto sphereDraw = createSphereDraw(context, fb);
    auto sphereVP = sphereDraw.uniform("ViewProjection");
    auto sphereT = sphereDraw.uniform("Transform");
    auto blitColor = deliberation::Blit(*fb.renderTarget(0),
                                        deliberation::Viewport(0, 0, 800, 450));
    auto blitNormalDepth = deliberation::Blit(*fb.renderTarget(1),
                                              deliberation::Viewport(800, 0, 800, 450));

    while (!glfwWindowShouldClose(window))
    {
        timer.lap();

        transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * timer.seconds(),
                                         glm::pi<float>() * 0.2f * timer.seconds(),
                                         glm::pi<float>() * 0.1f * timer.seconds()}));

        sphereVP.set(camera.viewProjection());
        sphereT.set(transform.matrix());

        bbClear.schedule();
        fbClear.schedule();
        sphereDraw.schedule();
        blitColor.schedule();
        blitNormalDepth.schedule();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deliberation::shutdown();

    glfwTerminate();

    return 0;
}

