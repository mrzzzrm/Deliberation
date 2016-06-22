#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Application
{
public:
    Application(const std::string & name);
    virtual ~Application();

    int run();

    void quit(int returnCode = 0);

protected:
    virtual void onStartup();
    virtual void onShutdown();
    virtual void onFrame(float seconds);

private:
    std::string     m_name;
    bool            m_running;
    int             m_returnCode;

    GLFWwindow *    m_window;
};

}

