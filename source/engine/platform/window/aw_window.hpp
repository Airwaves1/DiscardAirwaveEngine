#pragma once
#include <memory>
#include <string>
#include "graphics/graphic_contex.hpp"

namespace Airwave
{
class AwWindow
{
  public:
    AwWindow(const AwWindow &)            = delete;
    AwWindow &operator=(const AwWindow &) = delete;

    virtual ~AwWindow() = default;

    static std::unique_ptr<AwWindow> Create(uint32_t width = 1280, uint32_t height = 720,
                                            std::string title = "AirwaveEngine");

    virtual bool shouldClose() = 0;
    virtual void pollEvents()  = 0;
    virtual void swapBuffers() = 0;

    virtual void *getNativeWindow() = 0;

    std::shared_ptr<GraphicContext> getGraphicContext() { return m_graphicContext; }

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    std::string getTitle() const { return m_title; }

  protected:
    AwWindow() = default;

    uint32_t m_width{1280};
    uint32_t m_height{720};

    std::string m_title{"AirwaveEngine"};

    std::shared_ptr<GraphicContext> m_graphicContext;
};

} // namespace Airwave