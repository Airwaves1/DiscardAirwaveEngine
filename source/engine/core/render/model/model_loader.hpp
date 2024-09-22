#pragma once

#include <string>
#include <filesystem>
#include "tiny_gltf/tiny_gltf.h"
#include "tiny_obj/tiny_obj_loader.h"
#include "render/geometry/geometry_utils.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/object/mesh_component.hpp"

namespace Airwave
{
class ModelLoader
{

  public:
    ModelLoader()  = default;
    ~ModelLoader() = default;

    int load(const std::string &path, const std::shared_ptr<AwEntity> &entity,
             std::vector<std::shared_ptr<MeshComponent>> &meshComponents);
    // 加载模型的公共接口
    void load(const std::string &path, std::vector<AWVertex> &vertices, std::vector<uint32_t> &indices,
              std::vector<std::shared_ptr<MaterialComponent>> &materialComponents);

    void processObjModel(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
                         std::vector<AWVertex> &vertices, std::vector<uint32_t> &indices);
    void processMaterials(const std::string &dir, const std::vector<tinyobj::material_t> &tinyMaterials,
                          std::vector<std::shared_ptr<MaterialComponent>> &materials);

    void printObjModelInfo(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
                           const std::vector<tinyobj::material_t> &materials);
    void printGltfModelInfo(const tinygltf::Model &model);
};

} // namespace Airwave
