#include "aw_glfw_window.hpp"
#include "utils/log.hpp"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "graphics/graphic_contex.hpp"
#include <vulkan/vulkan.h>
#include "event/application_event.hpp"
#include "event/event_dispatcher.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"

namespace Airwave
{
AwGLFWwindow::AwGLFWwindow(uint32_t width, uint32_t height, std::string title)
{
    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return;
    }

#ifdef AW_ENGINE_GRAPHICS_API_VULKAN
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // 不使用OpenGL
#elif defined(AW_ENGINE_GRAPHICS_API_OPENGL)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // 使用OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);    // OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);    // OpenGL版本
#endif

    glfwWindowHint(GLFW_VISIBLE, false); // 可见性

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

    glfwMakeContextCurrent(m_glfwWindow); // 设置当前上下文

    setupWindowCallbacks();

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

void AwGLFWwindow::getMousePosition(double &x, double &y)
{
    glfwGetCursorPos(m_glfwWindow, &x, &y);
}

bool AwGLFWwindow::isMouseDown(MouseButton mouseButton)
{
    return glfwGetMouseButton(m_glfwWindow, mouseButton) == GLFW_PRESS;
}

bool AwGLFWwindow::isMouseUp(MouseButton mouseButton)
{
    return glfwGetMouseButton(m_glfwWindow, mouseButton) == GLFW_RELEASE;
}

bool AwGLFWwindow::isKeyDown(int key) { return glfwGetKey(m_glfwWindow, key) == GLFW_PRESS; }

bool AwGLFWwindow::isKeyUp(int key) { return glfwGetKey(m_glfwWindow, key) == GLFW_RELEASE; }

void AwGLFWwindow::setupWindowCallbacks()
{
    glfwSetWindowUserPointer(m_glfwWindow, this);

    glfwSetFramebufferSizeCallback(m_glfwWindow,
                                   [](GLFWwindow *window, int width, int height)
                                   {
                                       auto *awWindow = static_cast<AwGLFWwindow *>(
                                           glfwGetWindowUserPointer(window));
                                       if (awWindow)
                                       {
                                           WindowResizeEvent event(width, height);
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                   });
    glfwSetWindowFocusCallback(m_glfwWindow,
                               [](GLFWwindow *window, int focused)
                               {
                                   auto *awWindow = static_cast<AwGLFWwindow *>(
                                       glfwGetWindowUserPointer(window));
                                   if (awWindow)
                                   {
                                       if (focused)
                                       {
                                           WindowFocusEvent event{};
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                       else
                                       {
                                           WindowLostFocusEvent event{};
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                   }
                               });

    glfwSetWindowCloseCallback(m_glfwWindow,
                               [](GLFWwindow *window)
                               {
                                   auto *awWindow = static_cast<AwGLFWwindow *>(
                                       glfwGetWindowUserPointer(window));
                                   if (awWindow)
                                   {
                                       WindowCloseEvent event{};
                                       EventDispatcher::GetInstance().dispatch(event);
                                   }
                               });

    glfwSetWindowPosCallback(m_glfwWindow,
                             [](GLFWwindow *window, int xpos, int ypos)
                             {
                                 auto *awWindow =
                                     static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
                                 if (awWindow)
                                 {
                                     WindowMoveEvent event(xpos, ypos);
                                     EventDispatcher::GetInstance().dispatch(event);
                                 }
                             });

    glfwSetKeyCallback(m_glfwWindow,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           auto *awWindow =
                               static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
                           if (awWindow)
                           {
                               switch (action)
                               {
                                   case GLFW_PRESS:
                                   {
                                       KeyPressedEvent event(key, 0);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       KeyReleasedEvent event(key);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                                   case GLFW_REPEAT:
                                   {
                                       KeyPressedEvent event(key, 1);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                               }
                           }
                       });

    glfwSetCharCallback(m_glfwWindow,
                        [](GLFWwindow *window, unsigned int keycode)
                        {
                            auto *awWindow =
                                static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
                            if (awWindow)
                            {
                                KeyTypedEvent event(keycode);
                                EventDispatcher::GetInstance().dispatch(event);
                            }
                        });

    glfwSetMouseButtonCallback(
        m_glfwWindow,
        [](GLFWwindow *window, int button, int action, int mods)
        {
            auto *awWindow = static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
            if (awWindow)
            {
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        double x, y;
                        glfwGetCursorPos(window, &x, &y);
                        MouseButtonPressedEvent event(static_cast<MouseButton>(button), static_cast<float>(x), static_cast<float>(y));
                        EventDispatcher::GetInstance().dispatch(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        double x, y;
                        glfwGetCursorPos(window, &x, &y);
                        MouseButtonReleasedEvent event(static_cast<MouseButton>(button), static_cast<float>(x), static_cast<float>(y));
                        EventDispatcher::GetInstance().dispatch(event);
                        break;
                    }
                }
            }
        });

    glfwSetCursorPosCallback(
        m_glfwWindow,
        [](GLFWwindow *window, double xpos, double ypos)
        {
            auto *awWindow = static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
            if (awWindow)
            {
                MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
                EventDispatcher::GetInstance().dispatch(event);
            }
        });

    glfwSetScrollCallback(
        m_glfwWindow,
        [](GLFWwindow *window, double xoffset, double yoffset)
        {
            auto *awWindow = static_cast<AwGLFWwindow *>(glfwGetWindowUserPointer(window));
            if (awWindow)
            {
                MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
                EventDispatcher::GetInstance().dispatch(event);
            }
        });
}

} // namespace Airwave
