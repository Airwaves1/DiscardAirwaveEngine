#include "aw_gl_contex.hpp"
#include "graphics/opengl/aw_gl_contex.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/log.hpp"

namespace Airwave
{

AwGLContext::AwGLContext(AwWindow *window) : m_windowHandle(static_cast<GLFWwindow *>(window->getNativeWindow()))
{
    glfwMakeContextCurrent(m_windowHandle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        AW_ASSERT(false, "Failed to load OpenGL functions");
    }

    LOG_INFO("OpenGL Graphics API Info:");
    std::string vendor   = (char *)glGetString(GL_VENDOR);
    std::string renderer = (char *)glGetString(GL_RENDERER);
    std::string version  = (char *)glGetString(GL_VERSION);

    LOG_INFO("Vendor: {0}", vendor);
    LOG_INFO("Renderer: {0}", renderer);
    LOG_INFO("Version: {0}", version);
}
AwGLContext::~AwGLContext() {}

void AwGLContext::swapBuffers() { glfwSwapBuffers(m_windowHandle); }

} // namespace Airwave
