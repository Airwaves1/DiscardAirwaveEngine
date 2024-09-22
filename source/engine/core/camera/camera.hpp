#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Airwave
{

class Camera
{
  public:
    Camera()          = default;
    virtual ~Camera() = default;

    // 返回视图矩阵，如果脏标记为true，则更新视图矩阵
    const glm::mat4 getViewMatrix()
    {
        if (m_dirty)
        {
            updateViewMatrix();
        }
        return m_viewMatrix;
    }

    // 更新视图矩阵, 并将脏标记设为false
    void updateViewMatrix()
    {
        m_viewMatrix =
            glm::mat4_cast(glm::inverse(m_rotation)) * glm::translate(glm::mat4(1.0f), -m_position);

        m_dirty = false;
    }

    // 返回投影矩阵
    virtual const glm::mat4 &getProjectionMatrix() const = 0;

    virtual void updateProjectionMatrix() = 0;

    virtual void setFarClip(float far) = 0;

    virtual void setNearClip(float near) = 0;

    // 设置位置
    void setPosition(const glm::vec3 &position)
    {
        if (m_position == position)
        {
            return;
        }
        m_position = position;
        m_dirty    = true;
    }

    // 设置旋转（四元数）
    void setRotation(const glm::quat &rotation)
    {
        if (m_rotation == rotation)
        {
            return;
        }
        m_rotation = rotation;
        m_dirty    = true;
    }

    // 旋转相机
    void rotate(const glm::quat &delta)
    {
        if (delta == glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        {
            return;
        }
        m_rotation = glm::normalize(delta * m_rotation);
        m_dirty    = true;
    }

    // 设置世界坐标系的上方向
    void setWorldUp(const glm::vec3 &worldUp)
    {
        if (m_worldUp == worldUp)
        {
            return;
        }
        m_worldUp = glm::normalize(worldUp);
        m_dirty   = true;
    }

    const glm::vec3 &getPosition() const { return m_position; }
    const glm::quat &getRotation() const { return m_rotation; }
    const glm::vec3 &getWorldUp() const { return m_worldUp; }
    const glm::vec3 getForwardDirection() const // 获取相机的“前”方向（摄像机朝向）
    {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }
    glm::vec3 getRightDirection() const // 获取相机的“右”方向
    {
        return glm::normalize(m_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }
    glm::vec3 getUpDirection() const // 获取相机的“上”方向
    {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

  protected:
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};       // 位置
    glm::quat m_rotation{1.0f, 0.0f, 0.0f, 0.0f}; // 旋转
    glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};        // 世界坐标系的上方向
    glm::mat4 m_viewMatrix{1.0f};                 // 视图矩阵
    glm::mat4 m_projectionMatrix{1.0f};           // 投影矩阵
    bool m_dirty = true;                          // 视图矩阵是否需要更新
};

class PerspectiveCamera : public Camera
{
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far)
        : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
    {
        updateProjectionMatrix();
    }

    void updateProjectionMatrix() override
    {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    const glm::mat4 &getProjectionMatrix() const override { return m_projectionMatrix; }

    void setFarClip(float far) override
    {
        if (m_far == far)
        {
            return;
        }
        m_far = far;
        updateProjectionMatrix();
    }

    void setNearClip(float near) override
    {
        if (m_near == near)
        {
            return;
        }
        m_near = near;
        updateProjectionMatrix();
    }

    void setFov(float fov)
    {
        if (m_fov == fov)
        {
            return;
        }
        m_fov = fov;
        updateProjectionMatrix();
    }

    void setAspect(float aspect)
    {
        if (m_aspect == aspect)
        {
            return;
        }
        m_aspect = aspect;
        updateProjectionMatrix();
    }

    float getFov() const { return m_fov; }
    float getAspect() const { return m_aspect; }
    float getNearClip() const { return m_near; }
    float getFarClip() const { return m_far; }

  private:
    float m_fov{45.0f};           // 视野
    float m_aspect{16.0f / 9.0f}; // 宽高比
    float m_near{0.1f};           // 近裁剪面
    float m_far{100.0f};          // 远裁剪面
};

class OrthographicCamera : public Camera
{
  public:
    OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
        : m_leftSide(left), m_rightSide(right), m_bottomSide(bottom), m_topSide(top), m_near(near),
          m_far(far)
    {
        updateProjectionMatrix();
    }

    void updateProjectionMatrix() override
    {
        m_projectionMatrix =
            glm::ortho(m_leftSide, m_rightSide, m_bottomSide, m_topSide, m_near, m_far);
    }

    const glm::mat4 &getProjectionMatrix() const override { return m_projectionMatrix; }

    void setBounds(float left, float right, float bottom, float top, float near, float far)
    {
        m_leftSide   = left;
        m_rightSide  = right;
        m_bottomSide = bottom;
        m_topSide    = top;
        m_near       = near;
        m_far        = far;
        updateProjectionMatrix();
    }

    void setFarClip(float far) override
    {
        if (m_far == far)
        {
            return;
        }
        m_far = far;
        updateProjectionMatrix();
    }

    void setNearClip(float near) override
    {
        if(m_near == near)
        {
            return;
        }
        m_near = near;
        updateProjectionMatrix();
    }

    float getLeftSide() const { return m_leftSide; }
    float getRightSide() const { return m_rightSide; }
    float getBottomSide() const { return m_bottomSide; }
    float getTopSide() const { return m_topSide; }
    float getNearClip() const { return m_near; }
    float getFarClip() const { return m_far; }

  private:
    float m_leftSide;
    float m_rightSide;
    float m_topSide;
    float m_bottomSide;
    float m_near;
    float m_far;
};
} // namespace Airwave