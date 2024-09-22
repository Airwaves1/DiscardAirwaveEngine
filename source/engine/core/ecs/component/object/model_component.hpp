#pragma once

#include "ecs/aw_component.hpp"
#include "ecs/component/object/mesh_component.hpp"
#include "render/model/model_loader.hpp"

namespace Airwave
{
struct ModelComponent
{
    std::shared_ptr<MeshComponent> rootMeshComponent;
    std::vector<std::shared_ptr<MeshComponent>> meshComponents;

    void load(const std::string &path, const std::shared_ptr<AwEntity> &entity)
    {
        auto modelEntity = entity->getScene()->createAwEntity(entity->getName() + "_model");
        modelEntity->setParent(entity);
        ModelLoader modelLoader;
        int ret = modelLoader.load(path, modelEntity, meshComponents);

        if (ret != 0)
        {
            LOG_ERROR("Failed to load model: {0}", path);
            return;
        }

        if (!meshComponents.empty()) // 检查是否加载了任何网格
        {
            rootMeshComponent = meshComponents[0];
        }
        else
        {
            LOG_WARN("No mesh components loaded for model: {0}", path);
        }

        LOG_DEBUG("Model loaded: {0}", path);
        LOG_DEBUG("Mesh components count: {0}", meshComponents.size());
    }
};
} // namespace Airwave
