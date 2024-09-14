#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "utils/log.hpp"

namespace Airwave
{
class Camera
{
  public:
    Camera()          = default;
    virtual ~Camera() = default;

    // 更新
    virtual void updateViewMatrix()       = 0;
    virtual void updateProjectionMatrix() = 0;

    // 视图矩阵
    virtual glm::mat4 getViewMatrix() const { return m_viewMatrix; }

    // 投影矩阵
    virtual glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }

    // 位置
    void setPosition(const glm::vec3 &position)
    {
        m_position = position;
    }
    const glm::vec3 &getPosition() const { return m_position; }

    // 上方向
    void setUp(const glm::vec3 &up)
    {
        m_up = up;
    }
    const glm::vec3 &getUp() const { return m_up; }

    // 右方向
    void setRight(const glm::vec3 &right)
    {
        m_right = right;
    }

    const glm::vec3 &getRight() const { return m_right; }

  protected:
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};          // 位置
    glm::vec3 m_up    = glm::vec3(0.0f, 1.0f, 0.0f); // 上方向
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f); // 右方向

    glm::mat4 m_viewMatrix{1.0f};
    glm::mat4 m_projectionMatrix{1.0f};
};

class PerspectiveCamera : public Camera
{
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far)
        : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far), Camera()
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void updateViewMatrix() override
    {
        m_viewMatrix = glm::lookAt(m_position, glm::cross(m_right, m_up), m_up);
    }

    void updateProjectionMatrix() override
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    void setFov(float fov)
    {
        m_fov = fov;
        updateProjectionMatrix();
    }

    void setAspect(float aspect)
    {
        m_aspect = aspect;
        updateProjectionMatrix();
    }

    void setNear(float near)
    {
        m_near = near;
        updateProjectionMatrix();
    }

    void setFar(float far)
    {
        m_far = far;
        updateProjectionMatrix();
    }

    float getFov() const { return m_fov; }
    float getAspect() const { return m_aspect; }
    float getNear() const { return m_near; }
    float getFar() const { return m_far; }

  private:
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
};

class OrthographicCamera : public Camera
{
  public:
    OrthographicCamera(float leftSide, float rightSide, float bottom, float top, float near, float far)
        : m_leftSide(leftSide), m_rightSide(rightSide), m_bottom(bottom), m_top(top), m_near(near), m_far(far),
          Camera()
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void updateViewMatrix() override
    {
        m_viewMatrix = glm::lookAt(m_position, glm::cross(m_right, m_up), m_up);
    }

    void updateProjectionMatrix() override
    {
        m_projectionMatrix = glm::ortho(m_leftSide, m_rightSide, m_bottom, m_top, m_near, m_far);
    }

    void setLeftSide(float leftSide)
    {
        m_leftSide = leftSide;
        updateProjectionMatrix();
    }

    void setRightSide(float rightSide)
    {
        m_rightSide = rightSide;
        updateProjectionMatrix();
    }

    void setBottom(float bottom)
    {
        m_bottom = bottom;
        updateProjectionMatrix();
    }

    void setTop(float top)
    {
        m_top = top;
        updateProjectionMatrix();
    }

    void setNear(float near)
    {
        m_near = near;
        updateProjectionMatrix();
    }

    void setFar(float far)
    {
        m_far = far;
        updateProjectionMatrix();
    }

    float getLeftSide() const { return m_leftSide; }
    float getRightSide() const { return m_rightSide; }
    float getBottom() const { return m_bottom; }
    float getTop() const { return m_top; }
    float getNear() const { return m_near; }
    float getFar() const { return m_far; }

  private:
    float m_leftSide;
    float m_rightSide;
    float m_bottom;
    float m_top;
    float m_near;
    float m_far;
};

} // namespace Airwave