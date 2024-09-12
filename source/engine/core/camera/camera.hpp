#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

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
        updateViewMatrix();
    }
    const glm::vec3 &getPosition() const { return m_position; }

    // 朝向
    void setForward(const glm::vec3 &forward)
    {
        m_forward = forward;
        updateViewMatrix();
    }
    const glm::vec3 &getForward() const { return m_forward; }

  protected:
    glm::vec3 m_position{0.0f, 0.0f, 0.0f}; // 位置
    glm::vec3 m_forward{0.0f, 0.0f, -1.0f}; // 朝向

    glm::mat4 m_viewMatrix{1.0f};
    glm::mat4 m_projectionMatrix{1.0f};
};

class PerspectiveCamera : public Camera
{
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far)
        : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void updateViewMatrix() override
    {
        m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, glm::vec3(0.0f, 1.0f, 0.0f));
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
    OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
        : m_left(left), m_right(right), m_bottom(bottom), m_top(top), m_near(near), m_far(far)
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void updateViewMatrix() override
    {
        m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void updateProjectionMatrix() override
    {
        m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
    }

    void setLeft(float left)
    {
        m_left = left;
        updateProjectionMatrix();
    }

    void setRight(float right)
    {
        m_right = right;
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

    float getLeft() const { return m_left; }
    float getRight() const { return m_right; }
    float getBottom() const { return m_bottom; }
    float getTop() const { return m_top; }
    float getNear() const { return m_near; }
    float getFar() const { return m_far; }

  private:
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_near;
    float m_far;
};

} // namespace Airwave