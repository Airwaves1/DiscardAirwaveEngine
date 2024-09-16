#pragma once

#include "utils/common.hpp"
#include "utils/log.hpp"
#include "window/aw_window.hpp"
#include "event/event_observer.hpp"
namespace Airwave
{

struct ApplicationSettings
{
    uint32_t width = 1200;
    uint32_t height = 900;
    std::string title = "Airwave Engine";
};

class Application
{
  public:
    Application()                               = default;
    virtual ~Application()                      = default;
    Application(const Application &)            = delete;
    Application &operator=(const Application &) = delete;

    void start(int argc, char **argv);
    void quit();
    void mainLoop();

    const ApplicationSettings &getSettings() const { return m_settings; }

  protected:
    virtual void onConfiguration(ApplicationSettings &settings) {}
    virtual void onInit() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRender() {}
    virtual void onDstroy() {}

    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_lastFrameTimePoint;

    std::unique_ptr<AwWindow> m_window;

    std::shared_ptr<EventObserver> m_eventObserver;
    
    std::shared_ptr<GraphicContext> m_graphicContext;

  private:
    void parseArguments(int argc, char **argv);

  private:
    ApplicationSettings m_settings;
    uint64_t m_frameIndex = 0;
    bool b_pause          = false;
};

} // namespace Airwave