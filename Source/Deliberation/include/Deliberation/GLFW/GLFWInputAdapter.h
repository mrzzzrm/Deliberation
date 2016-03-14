#pragma once

#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/InputAdapterBase.h>

namespace deliberation
{

class DELIBERATION_API GLFWInputAdapter final:
    public deliberation::InputAdapterBase
{
public:
    GLFWInputAdapter(GLFWwindow * window);

    void update();

private:
    GLFWwindow * m_window;
};

}

