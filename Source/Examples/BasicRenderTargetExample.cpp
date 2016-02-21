#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Core/LapTimer.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Core/Math/Transform3D.h>

GLFWwindow * window;

deliberation::Draw createOffscreenDraw(deliberation::Context & context)
{
    auto program = context.createProgram({
       deliberation::dataPath("Data/Examples/Offscreen.vert"),
       deliberation::dataPath("Data/Examples/Offscreen.frag")
    });

    deliberation::UVSphere sphere(7, 7);
    auto mesh = sphere.generate();

    deliberation::MeshCompiler<deliberation::UVSphere::Vertex> compiler(mesh);
    compiler.compile();

    auto layout = context.createBufferLayout<deliberation::UVSphere::Vertex>({
        {"Position", &deliberation::UVSphere::Vertex::position},
        {"Normal", &deliberation::UVSphere::Vertex::normal}
    });

    auto depth = context.createTexture2D(300, 300, deliberation::PixelFormat_Depth_32F, false);
    auto texture0 = context.createTexture2D(300, 300, deliberation::PixelFormat_RGB_32F);
    auto texture1 = context.createTexture2D(300, 300, deliberation::PixelFormat_RGB_8U);

    auto draw = context.createDraw(program, gl::GL_TRIANGLES);
    draw.addVertices(layout, compiler.vertices());
    draw.setIndices32(compiler.indices());
    draw.framebuffer().setDepthTarget(&depth.surface());
    draw.framebuffer().setRenderTarget(0, &texture0.surface());
    draw.framebuffer().setRenderTarget(1, &texture1.surface());
    draw.state().setCullState(deliberation::CullState::disabled());

    return draw;
}

deliberation::Draw createOnscreenDraw(deliberation::Context & context,
                                      const deliberation::Surface & surface,
                                      unsigned int index)
{
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 uv;
    };
    auto layout = context.createBufferLayout<Vertex>({
        {"Position", &Vertex::position},
        {"UV", &Vertex::uv},
    });

    float width = 0.6f;

    auto vertices = std::vector<Vertex>({
        {{-1.0f + index * width, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f + width + index * width, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f + width + index * width, 0.5f}, {1.0f, 1.0f}},
        {{-1.0f + index * width, 0.5f}, {0.0f, 1.0f}}
    });

    auto indices = std::vector<unsigned int>({
        0, 1, 2,
        0, 2, 3
    });

    auto program = context.createProgram
    ({
        deliberation::dataPath("Data/Examples/Onscreen.vert"),
        deliberation::dataPath("Data/Examples/Onscreen.frag")
    });

    auto sourceTexture = surface.texture();

    auto onscreenDraw = context.createDraw(program, gl::GL_TRIANGLES);
    onscreenDraw.sampler("Texture").setTexture(sourceTexture);
    onscreenDraw.addVertices(layout, vertices);
    onscreenDraw.setIndices32(indices);
    onscreenDraw.state().setDepthState(deliberation::DepthState(false, false));
    onscreenDraw.state().setCullState(deliberation::CullState::disabled());

    return onscreenDraw;
}

deliberation::Clear createOffscreenClear(deliberation::Context & context, deliberation::Draw & draw)
{
    auto clear = context.createClear();
    clear.setFramebuffer(draw.framebuffer());

    return clear;
}

int main(int argc, char * argv[])
{
    std::cout << "---- BasicRenderTargetExample ----" << std::endl;

    // Init GLFW
    {
        if (!glfwInit())
        {
            return -1;
        }
        window = glfwCreateWindow(640, 480, "BasicRenderTargetExample", NULL, NULL);
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

    auto offscreenDraw = createOffscreenDraw(context);
    auto onscreenDraw0 = createOnscreenDraw(context, *offscreenDraw.framebuffer().renderTarget(0), 0);
    auto onscreenDraw1 = createOnscreenDraw(context, *offscreenDraw.framebuffer().renderTarget(1), 1);
    auto offscreenClear = createOffscreenClear(context, offscreenDraw);

    deliberation::Transform3D transform;
    deliberation::LapTimer timer;

    deliberation::Optional<deliberation::SurfaceDownload> download;
    deliberation::Optional<deliberation::Draw> downloadedDraw;

    while (!glfwWindowShouldClose(window))
    {
        timer.lap();

        transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * timer.seconds(),
                                         glm::pi<float>() * 0.2f * timer.seconds(),
                                         glm::pi<float>() * 0.1f * timer.seconds()}));
        offscreenDraw.uniform("transform").set(transform.matrix());

        offscreenClear.schedule();
        offscreenDraw.schedule();
        onscreenDraw0.schedule();
        onscreenDraw1.schedule();

        if (downloadedDraw.engaged())
        {
            downloadedDraw.get().schedule();
        }

        if (!download.engaged())
        {
            download.reset(offscreenDraw.framebuffer().renderTarget(1)->download());
            download.get().start();
        }
        else
        {
            if (download.get().isDone() && !downloadedDraw.engaged())
            {
                std::cout << download.get().result().toString() << std::endl;

                auto & surfaceBinary = download.get().result();
                deliberation::TextureBinary textureBinary(surfaceBinary);
                auto texture = context.createTexture(textureBinary);

                downloadedDraw.reset(createOnscreenDraw(context, texture.surface(), 2));
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deliberation::shutdown();

    glfwTerminate();

    return 0;
}

