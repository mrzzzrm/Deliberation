#include <Deliberation/GLFW/GLFWInputAdapter.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

//GLFWInputAdapter::GLFWInputAdapter(GLFWwindow * window):
//    m_window(window)
//{
//
//}
//
//void GLFWInputAdapter::update()
//{
//    if (!m_window)
//    {
//        m_window = glfwGetCurrentContext();
//    }
//
//    Assert(m_window, "");
//
//    for (auto glfwKey = GLFW_KEY_A; glfwKey <= GLFW_KEY_Z; glfwKey++)
//    {
//        auto state = glfwGetKey(m_window, glfwKey);
//        setKeyDown(Key_A + (glfwKey - GLFW_KEY_A), state == GLFW_PRESS);
//    }
//
//    double posx;
//    double posy;
//    glfwGetCursorPos(m_window, &posx, &posy);
//    setMousePosition({posx, posy});
//
//    setMouseButtonDown(MouseButton_Left, glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
//    setMouseButtonDown(MouseButton_Right, glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
//}

}

