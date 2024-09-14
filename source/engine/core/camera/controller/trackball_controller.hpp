#pragma once

#include "camera/camera.hpp"
#include <memory>
#include <algorithm>
#include "event/event_observer.hpp"

namespace Airwave
{
class TrackballController
{
  public:
    TrackballController(std::shared_ptr<Camera> camera = nullptr,
                        const glm::vec3 &target = glm::vec3(0.0f), float distance = 10.0f,
                        float sensitivity = 0.2f, float zoomSpeed = 0.2f, float minDistance = 1.0f,
                        float maxDistance = 100.0f);

    void setCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

    void setTarget(const glm::vec3 &target) { m_target = target; }
    const glm::vec3 &getTarget() const { return m_target; }

    // 旋转
    void rotate(float deltaX, float deltaY);

    // 缩放
    void zoom(float delta);

    // 平移
    void pan(float deltaX, float deltaY);

    const std::shared_ptr<EventObserver> &getEventObserver() const { return m_eventObserver; }

    // 设置基本事件响应
    void setupBasicEvent();


  private:
    std::shared_ptr<Camera> m_camera;
    glm::vec3 m_target{0.0f};
    float m_distance{10.0f};
    float m_sensitivity{0.2f};
    float m_zoomSpeed{0.2f};
    float m_minDistance{1.0f};
    float m_maxDistance{100.0f};

    std::shared_ptr<EventObserver> m_eventObserver;
};


} // namespace Airwave