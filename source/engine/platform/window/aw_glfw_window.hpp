#pragma once

#include "aw_window.hpp"

class GLFWwindow;
namespace Airwave
{


class AwGLFWwindow : public AwWindow
{
  public:
    AwGLFWwindow() = delete;
    AwGLFWwindow(uint32_t width, uint32_t height, std::string title);
    ~AwGLFWwindow() override;

    bool shouldClose() override;

    void pollEvents() override;

    void swapBuffers() override;

    void *getNativeWindow() override { return m_glfwWindow; }

  private:
    GLFWwindow *m_glfwWindow{nullptr};
};
} // namespace Airwave