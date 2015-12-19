#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawOutput.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

GLFWwindow * window;

deliberation::Draw createOffscreenDraw(deliberation::Context & context)
{
    struct Vertex
    {
        glm::vec2 position;
        glm::vec3 color;
    };

    deliberation::BufferLayout layout = context.createBufferLayout<Vertex>({
        {"Position", &Vertex::position},
        {"Color", &Vertex::color},
    });

    deliberation::Buffer buffer = context.createBuffer(layout);

    auto vertices = std::vector<Vertex>({
        {{-0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    });
    buffer.createUpload(vertices).schedule();
    deliberation::Program program = context.createProgram
    ({
        deliberation::dataPath("Data/Examples/Offscreen.vert"),
        deliberation::dataPath("Data/Examples/Offscreen.frag")
    });

    auto texture0 = context.createTexture2D(300, 300, deliberation::PixelFormat_RGB_32F);
    auto texture1 = context.createTexture2D(300, 300, deliberation::PixelFormat_RGB_32F);

    auto offscreenDraw = context.createDraw(program, gl::GL_TRIANGLES);
    offscreenDraw.addVertexBuffer(buffer);
    offscreenDraw.state().setDepthState(deliberation::DepthState(false, false));
    offscreenDraw.state().setCullState(deliberation::CullState::disabled());
    offscreenDraw.output().setRenderTarget(0, &texture0.surface());
    offscreenDraw.output().setRenderTarget(1, &texture1.surface());

    return offscreenDraw;
}

deliberation::Draw createOnscreenDraw(deliberation::Context & context,
                                      deliberation::Draw & offscreenDraw,
                                      int renderTargetIndex)
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

    auto vbuffer = context.createBuffer(layout);
    auto vertices = std::vector<Vertex>({
        {{-1.0f + renderTargetIndex * 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.2f + renderTargetIndex * 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.2f + renderTargetIndex * 1.0f, 0.5f}, {1.0f, 1.0f}},
        {{-1.0f + renderTargetIndex * 1.0f, 0.5f}, {0.0f, 1.0f}}
    });
    vbuffer.createUpload(vertices).schedule();

    auto ibuffer = context.createIndexBuffer8();
    auto indices = std::vector<gl::GLbyte>({
        0, 1, 2,
        0, 2, 3
    });
    ibuffer.createUpload(indices).schedule();

    auto program = context.createProgram
    ({
        deliberation::dataPath("Data/Examples/Onscreen.vert"),
        deliberation::dataPath("Data/Examples/Onscreen.frag")
    });

    auto sourceTexture = offscreenDraw.output().renderTarget(renderTargetIndex)->texture();

    auto onscreenDraw = context.createDraw(program, gl::GL_TRIANGLES);
    onscreenDraw.sampler("Texture").setTexture(sourceTexture);
    onscreenDraw.addVertexBuffer(vbuffer);
    onscreenDraw.setIndexBuffer(ibuffer);
    onscreenDraw.state().setDepthState(deliberation::DepthState(false, false));
    onscreenDraw.state().setCullState(deliberation::CullState::disabled());

    return onscreenDraw;
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
    deliberation::setPrefixPath("..");

    deliberation::Context context;

    auto offscreenDraw = createOffscreenDraw(context);
    auto onscreenDraw0 = createOnscreenDraw(context, offscreenDraw, 0);
    auto onscreenDraw1 = createOnscreenDraw(context, offscreenDraw, 1);

    while (!glfwWindowShouldClose(window))
    {
        offscreenDraw.schedule();
        onscreenDraw0.schedule();
        onscreenDraw1.schedule();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deliberation::shutdown();

    glfwTerminate();

    return 0;
}

