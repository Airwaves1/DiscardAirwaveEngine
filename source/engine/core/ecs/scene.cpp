#include "scene.hpp"
#include "node.hpp"
#include "aw_entity.hpp"
#include "system/system.hpp"

namespace Airwave
{
Scene::Scene(const std::string &name)
{
    m_rootNode = std::make_shared<Node>();
    m_rootNode->setName(name);
}

Scene::~Scene()
{
    m_rootNode.reset();
    destroyAllAwEntities();
    m_registry.clear();
}

std::shared_ptr<AwEntity> Scene::createAwEntity(const std::string &name)
{
    auto entity           = m_registry.create();
    auto awEntity         = std::make_shared<AwEntity>(entity, shared_from_this());
    m_awEntityMap[entity] = awEntity;
    awEntity->setParent(m_rootNode);
    awEntity->setName(name);
    return awEntity;
}

std::shared_ptr<AwEntity> Scene::getAwEntity(entt::entity entity)
{
    auto it = m_awEntityMap.find(entity);
    if (it != m_awEntityMap.end())
    {
        return it->second;
    }
    return nullptr;
}

void Scene::destroyAwEntity(const std::shared_ptr<AwEntity> &entity)
{
    if (entity && m_registry.valid(entity->getEntity()))
    {
        m_registry.destroy(entity->getEntity());
    }

    auto it = m_awEntityMap.find(entity->getEntity());
    if (it != m_awEntityMap.end())
    {
        if (entity->hasParent())
        {
            entity->getParent()->removeChild(entity);
        }
        m_awEntityMap.erase(it);
    }
}

void Scene::destroyAllAwEntities()
{
    for (const auto &[entity, awEntity] : m_awEntityMap)
    {
        if (m_registry.valid(entity))
        {
            m_registry.destroy(entity);
        }
    }
    m_awEntityMap.clear();
}

void Scene::addSystem(const std::shared_ptr<System> &system) { m_systems.insert(system); }

void Scene::removeSystem(const std::shared_ptr<System> &system) { m_systems.erase(system); }

void Scene::updateSystems(float deltaTime)
{
    for (const auto &system : m_systems)
    {
        system->update(shared_from_this(), deltaTime);
    }
}

} // namespace  Airwave
