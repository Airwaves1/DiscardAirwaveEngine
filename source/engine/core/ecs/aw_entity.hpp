#pragma once
#include "node.hpp"
#include "scene.hpp"
#include "utils/log.hpp"
#include <entt/entity/entity.hpp>
#include <memory>

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
    AwEntity(const entt::entity &entity, const std::shared_ptr<Scene> &scene) : m_entity(entity), m_scene(scene) {}
    ~AwEntity() {}

    bool operator==(const AwEntity &other) const { return m_entity == other.m_entity && m_scene == other.m_scene; }

    bool operator!=(const AwEntity &other) const { return !(*this == other); }

    bool isValid() const { return m_scene && m_scene->m_registry.valid(m_entity); }
    const entt::entity &getEntity() const { return m_entity; }

    // 判断是否包含某个组件
    template <typename T> bool hasComponent() const { return m_scene->m_registry.any_of<T>(m_entity); }

    // 判断是否是否包含任意一个组件
    template <typename... Args> bool hasAnyComponent() const { return m_scene->m_registry.any_of<Args...>(m_entity); }

    // 判断是否包含全部组件
    template <typename... Args> bool hasAllComponents() const { return m_scene->m_registry.all_of<Args...>(m_entity); }

    // 添加组件
    template <typename T, typename... Args> std::shared_ptr<T> addComponent(Args &&...args)
    {
        // 使用 std::shared_ptr 管理组件的生命周期
        auto &component = m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
        return std::shared_ptr<T>(&component, [](T *) { /* do nothing */ }); // 自定义删除器
    }

    template <typename T> void addEixstComponent(std::shared_ptr<T> component)
    {
        // 检查该实体是否已经有该类型的组件
        if (m_scene->m_registry.any_of<T>(m_entity))
        {
            // 如果存在，替换旧组件
            m_scene->m_registry.replace<T>(m_entity, *component);
        }
        else
        {
            // 否则添加新组件
            m_scene->m_registry.emplace<T>(m_entity, *component);
        }
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
    template <typename T> std::shared_ptr<T> getComponent()
    {
        if (hasComponent<T>())
        {
            // 获取已存在的组件
            return std::make_shared<T>(m_scene->m_registry.get<T>(m_entity));
        }
        else
        {
            LOG_ERROR("Entity does not have this component.");
            return nullptr; // 返回 nullptr 表示未找到组件
        }
    }

    // 获取所有组件的数组
    template <typename T> std::vector<std::shared_ptr<T>> getAllComponents()
    {
        std::vector<std::shared_ptr<T>> components;
        auto view = m_scene->m_registry.view<T>();
        for (auto entity : view)
        {
            components.push_back(std::make_shared<T>(view.get(entity))); // 获取并共享组件
        }
        return components;
    }

    std::shared_ptr<Scene> getScene() const { return m_scene; }

  private:
    entt::entity m_entity{entt::null};
    std::shared_ptr<Scene> m_scene;
};

} // namespace Airwave
