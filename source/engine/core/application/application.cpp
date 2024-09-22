#include "application.hpp"
#include "event/application_event.hpp"
#include "render/renderer/render_command.hpp"

//----------imgui----------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Airwave
{

void Application::start(int argc, char **argv)
{
    // 初始化日志
    Airwave::Log::Init();

    // 解析命令行参数
    parseArguments(argc, argv);
    onConfiguration(m_settings);

    // 创建窗口
    m_window = AwWindow::Create(m_settings.width, m_settings.height, m_settings.title);

    // 创建图形上下文
    m_graphicContext = GraphicContext::Create(m_window.get());

    // 初始化
    onInit();

    // 设置基本的事件
    m_eventObserver = std::make_shared<EventObserver>();
    m_eventObserver->onEvent<WindowResizeEvent>(
        [this](const WindowResizeEvent &event)
        {
            m_settings.width  = static_cast<uint32_t>(event.getWindowWidth());
            m_settings.height = static_cast<uint32_t>(event.getWindowHeight());

            LOG_DEBUG("WindowResizeEvent: width = {0}, height = {1}", m_settings.width,
                      m_settings.height);
            RenderCommand::OnViewportResize(2560, 1369);
        });

    m_startTimePoint = std::chrono::steady_clock::now();

    // imgui
    // 创建 ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // 配置 ImGui 样式
    ImGui::StyleColorsDark(); // 你也可以使用 Light 主题：ImGui::StyleColorsLight();

    // 初始化 ImGui 的 GLFW 平台/窗口绑定
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(m_window->getNativeWindow()), true);

    // 初始化 ImGui 的 OpenGL3 渲染绑定（"#version 130" 或更高）
    ImGui_ImplOpenGL3_Init("#version 450");
}

void Application::quit()
{
    onDstroy();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void Application::mainLoop()
{
    m_lastFrameTimePoint = std::chrono::steady_clock::now();
    while (!m_window->shouldClose())
    {
        m_window->pollEvents();

        float deltaTime =
            std::chrono::duration<float>(std::chrono::steady_clock::now() - m_lastFrameTimePoint)
                .count();
        m_lastFrameTimePoint = std::chrono::steady_clock::now();
        m_frameIndex++;

        if (!b_pause)
        {
            onUpdate(deltaTime);
            onRender();
        }

        // imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        onImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->swapBuffers();
    }
}

void Application::parseArguments(int argc, char **argv) {}

} // namespace Airwave
