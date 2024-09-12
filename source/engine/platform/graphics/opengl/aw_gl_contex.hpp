#pragma once
#include "window/aw_window.hpp"
#include "graphics/graphic_contex.hpp"

class GLFWwindow;
namespace Airwave
{
class AwGLContext : public GraphicContext
{
  public:
    AwGLContext() = delete;
    AwGLContext(AwWindow *window);
    ~AwGLContext();

    void swapBuffers();
  private:
    GLFWwindow *m_windowHandle;
};

} // namespace Airwave
