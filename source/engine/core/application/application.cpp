#include "application.hpp"
#include "event/application_event.hpp"
#include "render/renderer/render_command.hpp"
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
    m_eventObserver->onEvent<WindowResizeEvent>([this](const WindowResizeEvent &event) {
        m_settings.width  = static_cast<uint32_t> (event.getWindowWidth());
        m_settings.height = static_cast<uint32_t> (event.getWindowHeight());

        LOG_DEBUG("WindowResizeEvent: width = {0}, height = {1}", m_settings.width, m_settings.height);
        RenderCommand::OnViewportResize(2560, 1369);

    });

    m_startTimePoint = std::chrono::steady_clock::now();
}

void Application::quit() { onDstroy(); }
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

        m_window->swapBuffers();
    }
}

void Application::parseArguments(int argc, char **argv) {}

} // namespace Airwave
