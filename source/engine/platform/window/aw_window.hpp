#pragma once
#include <memory>
#include <string>
#include "event/event.hpp"
#include "event/mouse_event.hpp"
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

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    std::string getTitle() const { return m_title; }

    virtual void getMousePosition(double &x, double &y)                   = 0;
    virtual bool isMouseDown(MouseButton mouseButton = MOUSE_BUTTON_LEFT) = 0;
    virtual bool isMouseUp(MouseButton mouseButton = MOUSE_BUTTON_LEFT)   = 0;
    virtual bool isKeyDown(int key)                                       = 0;
    virtual bool isKeyUp(int key)                                         = 0;

  protected:
    AwWindow() = default;

    uint32_t m_width{1280};
    uint32_t m_height{720};

    std::string m_title{"AirwaveEngine"};

};

} // namespace Airwave