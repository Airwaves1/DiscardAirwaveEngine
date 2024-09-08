#include "aw_glfw_window.hpp"
#include "utils/log.hpp"

#include <vulkan/vulkan.h>


namespace Airwave
{
AwGLFWwindow::AwGLFWwindow(uint32_t width, uint32_t height, std::string title)
{
    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // 不使用OpenGL
    glfwWindowHint(GLFW_VISIBLE, false);          // 可见性

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_glfwWindow)
    {
        LOG_ERROR("Failed to create GLFW window");
        return;
    }

    m_width  = width;
    m_height = height;
    m_title  = title;

    // 设置窗口在屏幕中心
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);


    LOG_INFO("Created GLFW window");
    glfwMakeContextCurrent(m_glfwWindow); // 设置当前上下文

    glfwShowWindow(m_glfwWindow); // 显示窗口
}

AwGLFWwindow::~AwGLFWwindow()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

bool AwGLFWwindow::shouldClose() { return glfwWindowShouldClose(m_glfwWindow); }

void AwGLFWwindow::pollEvents()
{
    glfwPollEvents();

    if (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_glfwWindow, GLFW_TRUE);
    }
}

void AwGLFWwindow::swapBuffers() { glfwSwapBuffers(m_glfwWindow); }

} // namespace Airwave
