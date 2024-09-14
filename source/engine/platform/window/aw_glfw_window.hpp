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

    void getMousePosition(double &x, double &y) override;
    bool isMouseDown(MouseButton mouseButton = MOUSE_BUTTON_LEFT) override;
    bool isMouseUp(MouseButton mouseButton = MOUSE_BUTTON_LEFT) override;
    bool isKeyDown(int key) override;
    bool isKeyUp(int key) override;


  private:
    void setupWindowCallbacks();

    GLFWwindow *m_glfwWindow{nullptr};
};
} // namespace Airwave