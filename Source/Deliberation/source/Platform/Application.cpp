#include <Deliberation/Platform/Application.h>

#include <glbinding/Binding.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/MainLoop.h>

namespace deliberation
{

Application::Application(const std::string & name):
    m_name(name),
    m_running(false),
    m_returnCode(0)
{
}

Application::~Application() = default;

int Application::run()
{
    m_running = true;

    // Init GLFW
    if (!glfwInit())
    {
        m_returnCode = -1;
    }
    m_window = glfwCreateWindow(1600, 900, m_name.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        m_returnCode =  -1;
    }
    glfwMakeContextCurrent(m_window);

    // Init glbinding
    glbinding::Binding::initialize();

    deliberation::init();
    deliberation::EnableGLErrorChecks();
    deliberation::setPrefixPath("/home/moritz/Coding/VoxelAdvent/Extern/Deliberation/");

    onStartup();

    MainLoop().run([this](float seconds)
    {
        onFrame(seconds);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        return m_running && !glfwWindowShouldClose(m_window);
    });

    onShutdown();

    return m_returnCode;
}

void Application::quit(int returnCode)
{
    m_running = false;
    m_returnCode = returnCode;
}

void Application::onStartup()
{

}

void Application::onShutdown()
{

}

void Application::onFrame(float seconds)
{

}

}

