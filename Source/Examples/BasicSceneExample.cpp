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

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/Transform3D.h>

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

    float o = 1.0f;
    float n = 1.0f;
    float m = 0.98f;
    float p = (n + m) / 2.0f;

    auto vertices = std::vector<Vertex>({
        // Front
        {{-m, -m, n}, {0.0f, 0.0f, o}},
        {{ m, -m, n}, {0.0f, 0.0f, o}},
        {{ m,  m, n}, {0.0f, 0.0f, o}},
        {{-m,  m, n}, {0.0f, 0.0f, o}},

        // Back
        {{-m, -m, -n}, {0.0f, 0.0f, -o}},
        {{ m, -m, -n}, {0.0f, 0.0f, -o}},
        {{ m,  m, -n}, {0.0f, 0.0f, -o}},
        {{-m,  m, -n}, {0.0f, 0.0f, -o}},

        // Left
        {{-n, -m, -m}, {-o, 0.0f, 0.0f}},
        {{-n, -m,  m}, {-o, 0.0f, 0.0f}},
        {{-n,  m,  m}, {-o, 0.0f, 0.0f}},
        {{-n,  m, -m}, {-o, 0.0f, 0.0f}},

        // Right
        {{ n, -m,  m}, { o, 0.0f, 0.0f}},
        {{ n, -m, -m}, { o, 0.0f, 0.0f}},
        {{ n,  m, -m}, { o, 0.0f, 0.0f}},
        {{ n,  m,  m}, { o, 0.0f, 0.0f}},

        // Top
        {{-m, n,  m}, {0.0f, o, 0.0f}},
        {{ m, n,  m}, {0.0f, o, 0.0f}},
        {{ m, n, -m}, {0.0f, o, 0.0f}},
        {{-m, n, -m}, {0.0f, o, 0.0f}},

        // Bottom
        {{-m, -n, -m}, {0.0f, -o, 0.0f}},
        {{ m, -n, -m}, {0.0f, -o, 0.0f}},
        {{ m, -n,  m}, {0.0f, -o, 0.0f}},
        {{-m, -n,  m}, {0.0f, -o, 0.0f}},

        // Corners
        {{-p, p, p}, {-o, o, o}}, // tlf
        {{ p, p, p}, { o, o, o}}, // trf
        {{ p,-p, p}, { o,-o, o}}, // brf
        {{-p,-p, p}, {-o,-o, o}}, // blf
    });

    auto indices = std::vector<unsigned int>({
        // Front
        0, 1, 2,
        0, 2, 3,

        // Back
        4, 5, 6,
        4, 6, 7,

        // Left
        8, 9,10,
        8,10,11,

        // Right
        12,13,14,
        12,14,15,

        // Top
        16,17,18,
        16,18,19,

        // Bottom
        20,21,22,
        20,22,23,

        // Front - Top
        3, 2,17,
        3,17,16,
        // Front - Top - Left
        3,16,10,
//         3,24,16,
//        10,24,16,
//         3,24,10,
        // Front - Top - Right
        2,17,15,
//         2,25,15,
//         2,25,17,
//        15,25,17,
        // Front - Bottom
        0, 1,23,
        1,23,22,
        // Front - Bottom - Left
        0,23, 9,
        // Front - Bottom - Right
        1,22,12,
        // Front - Left
        0, 3,10,
        0,10, 9,
        // Front - Right
        1, 2,15,
        1,15,12,


        // Back - Top
        6, 7,19,
        6,19,18,
        // Back - Top - Left
        7,11,19,
        // Back - Top - Right
        6,14,18,
        // Back - Bottom
        4, 5,21,
        4,21,20,
        // Back - Bottom - Left
        8, 4,20,
        // Back - Bottom - Right
        5,13,21,
        // Back - Left
        4, 7,11,
        4,11, 8,
        // Back - Right
        5, 6,14,
        5,14,13,

        // Top - Left
        10,11,19,
        10,19,16,

        // Top - Right
        17,18,14,
        17,14,15,

        // Bottom - Left
        8, 9,23,
        8,23,20,

        // Bottom - Right
        21,22,12,
        21,12,13
    });

    auto layout = context.createBufferLayout<Vertex>({
        {"Position", &Vertex::position},
        {"Normal", &Vertex::normal}
    });

    auto vertexBuffer = context.createBuffer(layout);
    vertexBuffer.createUpload(vertices).schedule();

    auto indexBuffer = context.createIndexBuffer32();
    indexBuffer.createUpload(indices).schedule();

    auto result = context.createDraw(program, gl::GL_TRIANGLES);
    result.addVertexBuffer(vertexBuffer);
    result.setIndexBuffer(indexBuffer);
    result.state().setCullState(deliberation::CullState::disabled());
//    result.output().setDepthTarget(&depthBuffer.surface());
//    result.output().setRenderTarget("Color", &colorBuffer.surface());

    return result;
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

//    depthBuffer = context.createTexture2D(640, 480, deliberation::PixelFormat_Depth_32F);
//    colorBuffer = context.createTexture2D(640, 480, deliberation::PixelFormat_RGB_32F);

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

        transform.worldRotate(glm::quat({0.0f, glm::pi<float>() * 0.3f * seconds.count(), 0.0f}));

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

