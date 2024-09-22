#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "ecs/aw_component.hpp"
#include "ecs/aw_entity.hpp"

namespace Airwave
{

struct TransformComponent
{
  public:
    TransformComponent()
        : m_position(0.0f), m_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_scale(1.0f),
          m_dirty(true)
    {
    }

    // 获取变换矩阵
    glm::mat4 getTransformMatrix()
    {
        if (m_dirty)
        {
            m_transformMatrix = calculateTransformMatrix();
            m_dirty           = false;
        }
        return m_transformMatrix;
    }

    // 设置位置
    void setPosition(const glm::vec3 &position)
    {
        if (m_position == position) return;
        m_position = position;
        m_dirty    = true;
    }

    // 设置旋转
    void setRotation(const glm::quat &rotation)
    {
        if (m_rotation == rotation) return;
        m_rotation = rotation;
        m_dirty    = true;
    }

    // 设置缩放
    void setScale(const glm::vec3 &scale)
    {
        if (m_scale == scale) return;
        m_scale = scale;
        m_dirty = true;
    }

    
    // 设置父变换组件
    void setParent(std::shared_ptr<AwEntity> parent)
    {
        m_transform_parent = parent;
    }
    
    std::shared_ptr<AwEntity> getParent() const
    {
        return m_transform_parent;
    }

    // 获取位置、旋转、缩放
    const glm::vec3 &getPosition() const { return m_position; }
    const glm::quat &getRotation() const { return m_rotation; }
    const glm::vec3 &getScale() const { return m_scale; }

    friend class TransformSystem;

  private:
    glm::vec3 m_position; // 位置
    glm::quat m_rotation; // 旋转
    glm::vec3 m_scale;    // 缩放

    bool m_dirty;                // 标记是否需要更新矩阵
    glm::mat4 m_transformMatrix; // 缓存的变换矩阵

    std::shared_ptr<AwEntity> m_transform_parent; // 父变换组件

    // 计算变换矩阵
    glm::mat4 calculateTransformMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 rotation    = glm::toMat4(m_rotation);
        glm::mat4 scale       = glm::scale(glm::mat4(1.0f), m_scale);
        return translation * rotation * scale;
    }
};

} // namespace Airwave
