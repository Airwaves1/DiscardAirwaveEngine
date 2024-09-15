#pragma once

#include "event/event.hpp"

namespace Airwave
{
enum MouseButton
{
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(float xPos, float yPos) : m_XPos(xPos), m_YPos(yPos) {}
    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouse)

    std::string ToString() const override
    {
        std::stringstream a;
        a << "MouseMovedEvent: xPos = " << GetXPos() << ", yPos = " << GetYPos();
        return a.str();
    }

    inline float GetXPos() const { return m_XPos; }
    inline float GetYPos() const { return m_YPos; }

  private:
    float m_XPos, m_YPos;
};

class MouseButtonEvent : public Event
{
  public:
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
    MouseButtonEvent(MouseButton button, float xPos, float yPos) : m_Button(button), m_XPos(xPos), m_YPos(yPos) {}
    virtual ~MouseButtonEvent() {}
    inline MouseButton GetMouseButton() const { return m_Button; };
    inline float GetXPos() const { return m_XPos; }
    inline float GetYPos() const { return m_YPos; }

  protected:
    MouseButton m_Button;
    float m_XPos, m_YPos;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(MouseButton button, float xPos, float yPos) : MouseButtonEvent(button, xPos, yPos) {}
    EVENT_CLASS_TYPE(MouseButtonPressed)

    MouseButton GetMouseButton() const { return m_Button; }

    std::string ToString() const override
    {
        std::stringstream a;
        a << "MouseButtonPressed";
        return a.str();
    }

  private:
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(MouseButton button, float xPos, float yPos) : MouseButtonEvent(button, xPos, yPos) {}
    EVENT_CLASS_TYPE(MouseButtonReleased)

    std::string ToString() const override
    {
        std::stringstream a;
        a << "MouseButtonReleased";
        return a.str();
    }

  private:
};

class MouseScrolledEvent : public Event
{
  public:
    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouseButton)

    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}
    std::string ToString() const override
    {
        std::stringstream a;
        a << "MouseScrolled: xOffset = " << GetXOffset() << ", yOffset = " << GetYOffset();
        return a.str();
    }

    inline float GetXOffset() const { return m_XOffset; }
    inline float GetYOffset() const { return m_YOffset; }

  private:
    float m_XOffset, m_YOffset;
};

} // namespace Airwave
