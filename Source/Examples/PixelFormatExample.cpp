//#include <iostream>
//
//#include <fstream>
//
//#include <glbinding/Binding.h>
//#include <glbinding/gl/enum.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtx/transform.hpp>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
//
//#include <Deliberation/Deliberation.h>
//
//#include <Deliberation/Core/LapTimer.h>
//#include <Deliberation/Core/Viewport.h>
//#include <Deliberation/Core/StreamUtils.h>
//#include <Deliberation/Core/Math/Transform3D.h>
//
//#include <Deliberation/Draw/Buffer.h>
//#include <Deliberation/Draw/Framebuffer.h>
//#include <Deliberation/Draw/Context.h>
//#include <Deliberation/Draw/Texture.h>
//#include <Deliberation/Draw/Surface.h>
//#include <Deliberation/Draw/Program.h>
//#include <Deliberation/Draw/PixelFormat.h>
//
//#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusion.h>
//#include <Deliberation/Scene/Blit.h>
//#include <Deliberation/Scene/Camera3D.h>
//#include <Deliberation/Scene/UVSphere.h>
//#include <Deliberation/Scene/Mesh.h>
//#include <Deliberation/Scene/RenderNode.h>
//#include <Deliberation/Scene/RenderPipeline.h>
//#include <Deliberation/Scene/MeshCompiler.h>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
//
//using namespace deliberation;
//
//GLFWwindow * window;
//
//struct Vertex
//{
//    glm::vec3 position;
//    glm::vec3 normal;
//};
//
//Mesh<Vertex> createBunnyMesh()
//{
//    auto path = deliberation::dataPath("Data/Examples/bunny.obj");
//
//    Mesh<Vertex>::Vertices vertices;
//    Mesh<Vertex>::Faces faces;
//
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    std::string err;
//
//    auto r = tinyobj::LoadObj(shapes, materials, err, path.c_str());
//    if (!err.empty())
//    {
//        std::cout << "Error: " << err << std::endl;
//        exit(1);
//    }
//
//    std::cout << "# shapes: " << shapes.size() << std::endl;
//
//    Assert(!shapes.empty(), "");
//    auto & shape = shapes[0];
//
//    for (auto f = 0; f < shape.mesh.indices.size() / 3; f++) {
//        Mesh<Vertex>::Face face;
//        face.indices.push_back(shape.mesh.indices[3*f+0]);
//        face.indices.push_back(shape.mesh.indices[3*f+1]);
//        face.indices.push_back(shape.mesh.indices[3*f+2]);
//
//        faces.push_back(face);
//    }
//
//    for (auto v = 0; v < shape.mesh.positions.size(); v++)
//    {
//        Vertex vertex;
//        vertex.position = glm::vec3(shape.mesh.positions[3*v+0],
//                                    shape.mesh.positions[3*v+1],
//                                    shape.mesh.positions[3*v+2]) * 10.0f;
////        vertex.normal = glm::vec3(shape.mesh.normals[3*v+0],
////                                  shape.mesh.normals[3*v+1],
////                                  shape.mesh.normals[3*v+2]);
//        vertices.push_back(vertex);
//    }
//
////        for (auto f = 0; f < shape.mesh.num_vertices.size(); f++)
////        {
////            auto numVertices = shape.mesh.num_vertices[f];
////
////            for (auto v )
////        }
////
//
//    return Mesh<Vertex>(vertices, faces);
//}
//
//deliberation::Draw createSphereDraw(deliberation::Context & context, deliberation::Framebuffer & framebuffer)
//{
//    auto program = context.createProgram({
//       deliberation::dataPath("Data/Examples/AmbientOcclusionExample.vert"),
//       deliberation::dataPath("Data/Examples/AmbientOcclusionExample.frag")
//    });
//
//    deliberation::UVSphere sphere(7, 7);
//    auto mesh = createBunnyMesh();//sphere.generate();
//    mesh.computeNormals();
//
//    deliberation::MeshCompiler<Vertex> compiler(mesh);
//    compiler.compile();
//
//    auto layout = context.createBufferLayout<Vertex>({
//        {"Position", &Vertex::position},
//        {"Normal", &Vertex::normal}
//    });
//
//    auto draw = context.createDraw(program, gl::GL_TRIANGLES);
//    draw.addVertices(layout, compiler.vertices());
//    draw.setIndices32(compiler.indices());
//
//    draw.setFramebuffer(framebuffer);
//
//    return draw;
//}
//
int main(int argc, char * argv[])
{
//    std::cout << "---- AmbientOcclusionExample ----" << std::endl;
//
//    // Init GLFW
//    {
//        if (!glfwInit())
//        {
//            return -1;
//        }
//        window = glfwCreateWindow(1600, 900, "AmbientOcclusionExample", NULL, NULL);
//        if (!window)
//        {
//            glfwTerminate();
//            return -1;
//        }
//        glfwMakeContextCurrent(window);
//    }
//
//    glbinding::Binding::initialize();
//
//    deliberation::init();
//    deliberation::setPrefixPath("/home/moritz/Coding/VoxelAdvent/Extern/Deliberation/");
//
//    deliberation::Context context(1600, 900);
//
//    deliberation::Camera3D camera;
//    camera.setPosition({0.0f, 1.5f, 2.0f});
//    camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
//    camera.setAspectRatio(800.0f/450.0f);
//
//    deliberation::Transform3D transform;
//
//    deliberation::LapTimer timer;
//
//    deliberation::Framebuffer fb = context.createFramebuffer(800, 450);
//    fb.addDepthTarget(deliberation::PixelFormat_Depth_32_F);
//    fb.addRenderTarget(deliberation::PixelFormat_RGB_8_U); // Color
//    fb.addRenderTarget(deliberation::PixelFormat_RGBA_16_U); // NormalDepth
//
//    auto bbClear = context.createClear();
//    auto fbClear = fb.createClear();
//    fbClear.setColor(1, {0.0f, 0.0f, 0.0f, 1.0f});
//    auto sphereDraw = createSphereDraw(context, fb);
//    auto sphereV = sphereDraw.uniform("View");
//    auto sphereP = sphereDraw.uniform("Projection");
//    auto sphereT = sphereDraw.uniform("Transform");
//    auto sphereFPZ = sphereDraw.uniform("FarPlaneZ");
//    auto blitColor = deliberation::Blit(*fb.renderTarget(0),
//                                        deliberation::Viewport(0, 450, 800, 450));
//    auto blitNormalDepth = deliberation::Blit(*fb.renderTarget(1),
//                                              deliberation::Viewport(800, 450, 800, 450));
//
//    deliberation::AmbientOcclusion ao(*fb.renderTarget(1), *fb.renderTarget(0), camera);
//
//    auto blitAO = deliberation::Blit(ao.output(), deliberation::Viewport(0, 0, 800, 450));
//
//    bool leftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
//
//    Optional<SurfaceDownload> download;
//    int surfaceX = 0;
//    int surfaceY = 0;
//    int surfaceBPP = 0;
//
//    while (!glfwWindowShouldClose(window))
//    {
//        timer.lap();
//
//        transform.worldRotate(glm::quat({glm::pi<float>() * 0.3f * timer.seconds() * 0.0f,
//                                         glm::pi<float>() * 0.2f * timer.seconds(),
//                                         glm::pi<float>() * 0.1f * timer.seconds() * 0.0f}));
//
//        sphereV.set(camera.view());
//        sphereP.set(camera.projection());
//        sphereT.set(transform.matrix());
//        sphereFPZ.set(camera.zFar());
//
//        bbClear.schedule();
//        fbClear.schedule();
//        sphereDraw.schedule();
//        blitColor.schedule();
//        blitNormalDepth.schedule();
//        ao.schedule();
//        blitAO.schedule();
//
////        {
////            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
////            if (state == GLFW_RELEASE && leftMouseButtonPressed)
////            {
////                double x, y;
////                glfwGetCursorPos(window, &x, &y);
////
////                std::cout << x << " " << y << std::endl;
////
////                if (x < 800 && y > 450)
////                {
////                    surfaceX = x;
////                    surfaceY = 900 - y;
////
////                    download = ao.output().download();
////                    download.get().start();
////                    surfaceBPP = 3;
////                }
////                if (x > 800 && y < 450)
////                {
////                    surfaceX = x - 800;
////                    surfaceY = 450 - y;
////
////                    download = fb.renderTarget(1)->download();
////                    download.get().start();
////                    surfaceBPP = 4;
////                }
////                if (x > 800 && y > 450)
////                {
////                    surfaceX = x - 800;
////                    surfaceY = 900 - y;
////
////                    download = fb.renderTarget(2)->download();
////                    download.get().start();
////                    surfaceBPP = 3;
////                }
////            }
////            leftMouseButtonPressed = state == GLFW_PRESS;
////
////            if (download.engaged())
////            {
////                if (download.get().isDone())
////                {
////                    auto & result = download.get().result();
////
////                    if (surfaceBPP == 3)
////                    {
////                        std::cout << "Color @" << glm::ivec2(surfaceX, surfaceY) <<  ": " << result.pixel<glm::vec3>(surfaceX, surfaceY) << std::endl;
////                    }
////                    if (surfaceBPP == 4)
////                    {
////                        std::cout << "Color @" << glm::ivec2(surfaceX, surfaceY) <<  ": " << result.pixel<glm::vec4>(surfaceX, surfaceY) << std::endl;
////                    }
////
////                    download.disengage();
////                }
////            }
////        }
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    deliberation::shutdown();
//
//    glfwTerminate();
//
    return 0;
}

