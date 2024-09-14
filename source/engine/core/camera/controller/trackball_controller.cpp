#include "trackball_controller.hpp"
#include "utils/log.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Airwave
{
TrackballController::TrackballController(std::shared_ptr<Camera> camera, const glm::vec3 &target,
                                         float distance, float sensitivity, float zoomSpeed,
                                         float minDistance, float maxDistance)
    : m_camera(camera), m_target(target), m_distance(distance), m_sensitivity(sensitivity),
      m_zoomSpeed(zoomSpeed)
{
    m_eventObserver = std::make_shared<EventObserver>();
    setupBasicEvent();
}

void TrackballController::rotate(float deltaX, float deltaY)
{
    // 绕right轴旋转
    auto rotateRight =
        glm::rotate(glm::mat4(1.0f), glm::radians(-deltaY * m_sensitivity), m_camera->getRight());
    // 影响当前相机的up和位置
    auto up = rotateRight * glm::vec4(m_camera->getUp(), 0.0f);
    m_camera->setUp(glm::vec3(up));
    auto position = rotateRight * glm::vec4(m_camera->getPosition() - m_target, 1.0f);
    m_camera->setPosition(glm::vec3(position) + m_target);

    // 绕up轴旋转
    auto rotateUp =
        glm::rotate(glm::mat4(1.0f), glm::radians(-deltaX * m_sensitivity), m_camera->getUp());
    // 影响当前相机的right和位置
    auto right = rotateUp * glm::vec4(m_camera->getRight(), 0.0f);
    m_camera->setRight(glm::vec3(right));
    position = rotateUp * glm::vec4(m_camera->getPosition() - m_target, 1.0f);
    m_camera->setPosition(glm::vec3(position) + m_target);

    LOG_DEBUG("rotate");
}

void TrackballController::zoom(float delta)
{
    m_distance = std::clamp(m_distance - delta * m_zoomSpeed, m_minDistance, m_maxDistance);

    glm::vec3 direction = m_camera->getPosition() - m_target;
    m_camera->setPosition(m_target + glm::normalize(direction) * m_distance);

    LOG_DEBUG("zoom");
    
}

void TrackballController::pan(float deltaX, float deltaY)
{
    if (!m_camera) return;

    // 获取相机的右方向和上方向
    glm::vec3 right = m_camera->getRight();
    glm::vec3 up = m_camera->getUp();
    
    // 计算平移量，乘以适当的敏感度来调整平移速度
    glm::vec3 move = right * deltaX * m_sensitivity + up * deltaY * m_sensitivity;

    // 在世界坐标系中平移目标点
    m_target += move;

    // 更新相机位置，保持相机与目标点的距离不变
    glm::vec3 direction = glm::normalize(m_camera->getPosition() - m_target);
    m_camera->setPosition(m_target + direction * m_distance);

    // 更新视图矩阵
    m_camera->updateViewMatrix();

    LOG_DEBUG("pan");
    
}


void TrackballController::setupBasicEvent()
{
    if (m_eventObserver)
    {
        static bool isLeftMousePressed   = false;
        static bool isMiddleMousePressed = false;

        m_eventObserver->onEvent<MouseScrolledEvent>([this](const MouseScrolledEvent &event)
                                                     { this->zoom(event.GetYOffset()); });

        // 处理鼠标按下事件（左键用于旋转，中键用于平移）
        m_eventObserver->onEvent<MouseButtonPressedEvent>(
            [this](const MouseButtonPressedEvent &event)
            {
                if (event.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT)
                {
                    isLeftMousePressed = true;
                }
                else if (event.GetMouseButton() == MouseButton::MOUSE_BUTTON_MIDDLE)
                {
                    isMiddleMousePressed = true;
                }
            });

        // 处理鼠标释放事件
        m_eventObserver->onEvent<MouseButtonReleasedEvent>(
            [this](const MouseButtonReleasedEvent &event)
            {
                if (event.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT)
                {
                    isLeftMousePressed = false;
                }
                else if (event.GetMouseButton() == MouseButton::MOUSE_BUTTON_MIDDLE)
                {
                    isMiddleMousePressed = false;
                }
            });

        // 处理鼠标移动事件
        m_eventObserver->onEvent<MouseMovedEvent>(
            [this](const MouseMovedEvent &event)
            {
                static glm::vec2 lastPos =
                    glm::vec2(event.GetXPos(), event.GetYPos()); // 初始化为当前位置

                glm::vec2 currentPos = glm::vec2(event.GetXPos(), event.GetYPos());
                glm::vec2 delta      = currentPos - lastPos;

                if (isLeftMousePressed)
                {
                    this->rotate(delta.x, delta.y);
                }
                else if (isMiddleMousePressed)
                {
                    this->pan(delta.x, delta.y);
                }

                lastPos = currentPos;
            });
    }
}

} // namespace Airwave
