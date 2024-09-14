#include "trackball_controller.hpp"

namespace Airwave
{
TrackballController::TrackballController(std::shared_ptr<Camera> camera, const glm::vec3 &target,
                                         float distance, float sensitivity, float zoomSpeed,
                                         float minDistance, float maxDistance)
    : m_camera(camera), m_target(target), m_distance(distance), m_sensitivity(sensitivity),
      m_zoomSpeed(zoomSpeed), m_minDistance(minDistance), m_maxDistance(maxDistance)
{
    m_eventObserver = std::make_shared<EventObserver>();
}

void TrackballController::rotate(float deltaX, float deltaY)
{
    glm::vec3 direction = m_camera->getPosition() - m_target;

    direction = glm::rotate(glm::mat4(1.0f), glm::radians(deltaX * m_sensitivity),
                            glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::vec4(direction, 1.0f);
    direction = glm::rotate(glm::mat4(1.0f), glm::radians(deltaY * m_sensitivity),
                            glm::cross(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f))) *
                glm::vec4(direction, 1.0f);

    m_camera->setPosition(m_target + direction);
    m_camera->updateViewMatrix();
}

void TrackballController::zoom(float delta)
{
    m_distance = std::clamp(m_distance - delta * m_zoomSpeed, m_minDistance, m_maxDistance);

    glm::vec3 direction = m_camera->getPosition() - m_target;
    m_camera->setPosition(m_target + glm::normalize(direction) * m_distance);
    m_camera->updateViewMatrix();
}

void TrackballController::pan(float deltaX, float deltaY) {
    glm::vec3 direction = m_camera->getPosition() - m_target;
    glm::vec3 right = glm::cross(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 up = glm::cross(right, glm::normalize(direction));

    glm::vec3 offset = right * deltaX * m_sensitivity + up * deltaY * m_sensitivity;
    m_target += offset;
    m_camera->setPosition(m_camera->getPosition() + offset);
    m_camera->updateViewMatrix();

}

void TrackballController::setupBasicEvent() {

}

} // namespace Airwave
