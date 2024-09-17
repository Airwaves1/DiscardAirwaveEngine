#pragma once
#include "node.hpp"
#include "scene.hpp"
#include "utils/log.hpp"
#include <entt/entity/entity.hpp>

/**
 * Entity类继承自Node，并封装了与ECS系统交互的功能。每个Entity代表一个场景中的实体，并能够管理其组件。
 * 功能:
        组件操作: 提供添加、获取、移除组件的功能。支持检查是否存在某个组件或某些组件。
        与场景交互: Entity持有对Scene的指针，通过该指针与entt::registry进行交互。
 */

namespace Airwave
{
class AwEntity : public Node
{
  public:
    AwEntity(const entt::entity &entity, const std::shared_ptr<Scene> &scene)
        : m_entity(entity), m_scene(scene)
    {
    }
    ~AwEntity() {}

    bool operator==(const AwEntity &other) const
    {
        return m_entity == other.m_entity && m_scene == other.m_scene;
    }

    bool operator!=(const AwEntity &other) const { return !(*this == other); }

    bool isValid() const { return m_scene && m_scene->m_registry.valid(m_entity); }
    const entt::entity &getEntity() const { return m_entity; }

    // 判断是否包含某个组件
    template <typename T> bool hasComponent() const { return m_scene->m_registry.any_of<T>(m_entity); }

    // 判断是否是否包含任意一个组件
    template <typename... Args> bool hasAnyComponent() const
    {
        return m_scene->m_registry.any_of<Args...>(m_entity);
    }

    // 判断是否包含全部组件
    template <typename... Args> bool hasAllComponents() const
    {
        return m_scene->m_registry.all_of<Args...>(m_entity);
    }

    // 添加组件
    template <typename T, typename... Args> T &addComponent(Args &&...args)
    {
        return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    // 移除组件
    template <typename T> void removeComponent()
    {
        if (hasComponent<T>())
            m_scene->m_registry.remove<T>(m_entity);
        else
            LOG_ERROR("removeComponent not implemented");
    }

    // 获取组件
    template <typename T> T &getComponent()
    {
        if (hasComponent<T>())
            return m_scene->m_registry.get<T>(m_entity);
        else
            LOG_ERROR("getComponent not implemented");
    }

  private:
    entt::entity m_entity{entt::null};
    std::shared_ptr<Scene> m_scene;
};

} // namespace Airwave
