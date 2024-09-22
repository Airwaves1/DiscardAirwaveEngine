#pragma once
#include "uuid.hpp"
#include <entt/entity/registry.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
/**
 *  Scene类用于管理一个场景，包含ECS系统中的所有实体和组件。它是实体管理的中心节点。
 *  功能:
        UUID和名称: 每个场景有一个唯一的UUID和名称。
        ECS注册表: 使用entt::registry来存储和管理实体及其组件。
        实体映射: 维护一个从entt::entity到Entity对象的映射，方便管理和访问实体。
        根节点: 包含一个根节点，用于构建场景的层级结构, 相当于scene也是一个node
 */

namespace Airwave
{
class Node;
class AwEntity;
class System;

class Scene : public std::enable_shared_from_this<Scene>
{
  public:
    Scene(const std::string &name = "Scene");
    ~Scene();

    // 实体操作
    std::shared_ptr<AwEntity> createAwEntity(const std::string &name = "AwEntity");
    std::shared_ptr<AwEntity> getAwEntity(entt::entity entity);
    std::shared_ptr<AwEntity> getAwEntity(const std::string &name);
    void destroyAwEntity(const std::shared_ptr<AwEntity> &entity);
    void destroyAllAwEntities();

    // 添加或移除系统
    void addSystem(const std::shared_ptr<System> &system);
    void removeSystem(const std::shared_ptr<System> &system);
    void updateSystems(float deltaTime);
    
    // 查看是否有某个系统
    template <typename T>
    bool hasSystem() const
    {
        const std::type_index typeIndex = std::type_index(typeid(T));
        return m_systems.find(typeIndex) != m_systems.end();
    }

    // 获取系统
    template <typename T>
    std::shared_ptr<T> getSystem()
    {
        const std::type_index typeIndex = std::type_index(typeid(T));
        if (m_systems.find(typeIndex) != m_systems.end())
        {
            return std::static_pointer_cast<T>(m_systems[typeIndex]);
        }
        return nullptr;
    }

    entt::registry &getRegistry() { return m_registry; } // ECS注册表
    std::shared_ptr<Node> getRootNode() const { return m_rootNode; }

    // 保存场景
    // bool save(const std::string &filename) {}
    // bool load(const std::string &filename) {}

  private:
    entt::registry m_registry; // ECS注册表

    std::unordered_map<entt::entity, std::shared_ptr<AwEntity>> m_awEntityMap; // 实体映射
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems; // 系统集合
    std::shared_ptr<Node> m_rootNode;                                            // 根节点

    friend class AwEntity;
};

} // namespace Airwave
