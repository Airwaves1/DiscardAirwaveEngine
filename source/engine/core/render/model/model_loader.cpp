#include "model_loader.hpp"
#include <iostream>
#include "utils/file_utils.hpp"
#include "tiny_gltf/tiny_gltf.h"
#include "ecs/aw_entity.hpp"
#include "ecs/component/transform/transform_component.hpp"
namespace Airwave
{
int ModelLoader::load(const std::string &path, const std::shared_ptr<AwEntity> &modelEntity,
                      std::vector<std::shared_ptr<MeshComponent>> &meshComponents)
{
    std::string extension = std::filesystem::path(path).extension().string();
    std::filesystem::path filePath(path);
    std::string directory = filePath.parent_path().string() + "/";

    if (extension == ".obj")
    {
        std::cout << "Loading OBJ file: " << path << std::endl;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str());
        if (!ret) return -1; // Error handling simplified

        // 使用共享顶点数据
        std::vector<AWVertex> sharedVertices;
        std::vector<uint32_t> sharedIndices;
        std::vector<std::shared_ptr<MaterialComponent>> materialComponents;
        processMaterials(directory, materials, materialComponents);

        // 将所有顶点数据存储到共享顶点向量中
        for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
        {
            AWVertex vertex{};
            vertex.position = {attrib.vertices[3 * v], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]};

            if (v < attrib.normals.size() / 3)
            {
                vertex.normal = {attrib.normals[3 * v], attrib.normals[3 * v + 1], attrib.normals[3 * v + 2]};
            }

            if (v < attrib.texcoords.size() / 2)
            {
                vertex.texCoord = {attrib.texcoords[2 * v], attrib.texcoords[2 * v + 1]};
            }

            sharedVertices.push_back(vertex);
        }

        // 处理每个 shape
        for (const auto &shape : shapes)
        {
            std::vector<uint32_t> indices;

            for (const auto &index : shape.mesh.indices)
            {
                AWVertex vertex{};

                // 顶点位置
                vertex.position = {attrib.vertices[3 * index.vertex_index], attrib.vertices[3 * index.vertex_index + 1],
                                   attrib.vertices[3 * index.vertex_index + 2]};

                // 法线
                if (index.normal_index >= 0)
                {
                    vertex.normal = {attrib.normals[3 * index.normal_index], attrib.normals[3 * index.normal_index + 1],
                                     attrib.normals[3 * index.normal_index + 2]};
                }

                // UV 坐标（纹理坐标）
                if (index.texcoord_index >= 0)
                {
                    vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index],
                                       attrib.texcoords[2 * index.texcoord_index + 1]};
                }

                // 添加到共享顶点
                sharedVertices.push_back(vertex);
                indices.push_back(sharedVertices.size() - 1); // 将顶点索引添加到索引数组中
            }

            auto geometryComponent = std::make_shared<GeometryComponent>();
            geometryComponent->setData(sharedVertices, indices); // 使用共享的顶点数据

            // 为每个 shape 创建 mesh 实体
            auto meshEntity = modelEntity->getScene()->createAwEntity(modelEntity->getName() + "_mesh_" + shape.name);
            meshEntity->setParent(modelEntity);
            auto transformComponent = meshEntity->addComponent<TransformComponent>();
            transformComponent->setParent(modelEntity);

            auto meshComponent               = meshEntity->addComponent<MeshComponent>();
            meshComponent->name              = shape.name;
            meshComponent->geometryComponent = geometryComponent;

            // 关联材质
            if (!shape.mesh.material_ids.empty()) // 确保材质ID列表非空
            {
                int materialIndex = shape.mesh.material_ids[0]; // 获取第一个材质ID
                if (materialIndex >= 0 && materialIndex < materialComponents.size())
                {
                    meshComponent->materialComponent = materialComponents[materialIndex]; // 关联材质
                }
                else
                {
                    LOG_WARN("Material index out of range for mesh: {0}, using default material", shape.name);
                    meshComponent->materialComponent = std::make_shared<MaterialComponent>(); // 使用默认材质
                }
            }

            meshComponents.push_back(meshComponent);
        }

        printObjModelInfo(attrib, shapes, materials);
    }
    return 0; // Return success code
}

void ModelLoader::load(const std::string &path, std::vector<AWVertex> &vertices, std::vector<uint32_t> &indices,
                       std::vector<std::shared_ptr<MaterialComponent>> &materialComponents)
{
    // 获取文件扩展名
    std::string extension = std::filesystem::path(path).extension().string();
    // 获取文件所在文件夹路径
    std::filesystem::path filePath(path);
    std::string directory = filePath.parent_path().string() + "/";

    if (extension == ".obj")
    {
        std::cout << "Current OBJ file is located in: " << directory << std::endl;

        std::cout << "Loading OBJ file: " << path << std::endl;
        std::cout << "Using base directory: " << std::filesystem::path(path).parent_path().string() << std::endl;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        // 使用基础路径
        std::string baseDir = std::filesystem::path(path).parent_path().string();
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), baseDir.c_str());

        if (!warn.empty())
        {
            std::cerr << "TinyObjLoader warning: " << warn << std::endl;
        }

        if (!err.empty())
        {
            std::cerr << "TinyObjLoader error: " << err << std::endl;
        }

        if (!ret)
        {
            std::cerr << "Failed to load OBJ file: " << path << std::endl;
            return;
        }

        // 转换为内部格式
        processObjModel(attrib, shapes, vertices, indices);
        processMaterials(directory, materials, materialComponents);

        // 打印模型信息
        // printObjModelInfo(attrib, shapes, materials);
    }

    else if (extension == ".gltf" || extension == ".glb")
    {
        // 加载 GLTF 模型
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string warn, err;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path.c_str());
        if (!warn.empty())
        {
            std::cerr << "TinyGLTF warning: " << warn << std::endl;
        }

        if (!err.empty())
        {
            std::cerr << "TinyGLTF error: " << err << std::endl;
        }

        if (!ret)
        {
            std::cerr << "Failed to load GLTF file: " << path << std::endl;
            return;
        }

        // 打印模型信息
        // printGltfModelInfo(model);
    }
    else
    {
        std::cerr << "Unsupported file format: " << extension << std::endl;
    }
}

void ModelLoader::processObjModel(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
                                  std::vector<AWVertex> &vertices, std::vector<uint32_t> &indices)
{
    std::unordered_map<uint32_t, uint32_t> uniqueVertices; // 用于合并相同的顶点
    uint32_t indexOffset = 0;

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            AWVertex vertex{};

            // 位置
            vertex.position = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                               attrib.vertices[3 * index.vertex_index + 2]};

            // 法线
            if (index.normal_index >= 0)
            {
                vertex.normal = {attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1],
                                 attrib.normals[3 * index.normal_index + 2]};
            }

            // 纹理坐标
            if (index.texcoord_index >= 0)
            {
                vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                                   attrib.texcoords[2 * index.texcoord_index + 1]};
            }

            // 判断是否需要创建新的顶点，避免重复顶点
            uint32_t vertexHash = index.vertex_index; // 可以根据需要扩展哈希计算
            if (uniqueVertices.find(vertexHash) == uniqueVertices.end())
            {
                uniqueVertices[vertexHash] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            // 将顶点索引加入索引数组
            indices.push_back(uniqueVertices[vertexHash]);
        }
    }
}

void ModelLoader::processMaterials(const std::string &dir, const std::vector<tinyobj::material_t> &tinyMaterials,
                                   std::vector<std::shared_ptr<MaterialComponent>> &materials)
{
    for (const auto &tinyMaterial : tinyMaterials)
    {
        std::shared_ptr<MaterialComponent> material = std::make_shared<MaterialComponent>();

        // 材质名称
        material->name = tinyMaterial.name;

        // 环境光颜色
        material->ambient = glm::vec3(tinyMaterial.ambient[0], tinyMaterial.ambient[1], tinyMaterial.ambient[2]);

        // 漫反射颜色
        material->diffuse = glm::vec3(tinyMaterial.diffuse[0], tinyMaterial.diffuse[1], tinyMaterial.diffuse[2]);

        // 镜面反射颜色
        material->specular = glm::vec3(tinyMaterial.specular[0], tinyMaterial.specular[1], tinyMaterial.specular[2]);

        // 反射率
        material->shininess = tinyMaterial.shininess;

        // 是否使用镜面高光贴图
        material->specularHighlight = tinyMaterial.illum == 2;

        // 漫反射贴图
        if (!tinyMaterial.diffuse_texname.empty())
        {
            material->diffuseMap = Texture2D::Create(dir + tinyMaterial.diffuse_texname);
        }

        // 镜面高光贴图
        if (!tinyMaterial.specular_texname.empty())
        {
            material->specularMap = Texture2D::Create(dir + tinyMaterial.specular_texname);
        }

        // 法线贴图
        if (!tinyMaterial.bump_texname.empty())
        {
            material->normalMap = Texture2D::Create(dir + tinyMaterial.bump_texname);
        }

        // 其他材质属性...
        // material.dissolve = tinyMaterial.dissolve; // 透明度
        // material.ior      = tinyMaterial.ior;      // 折射率

        // 添加到材质列表
        materials.push_back(material);
    }
}

void ModelLoader::printObjModelInfo(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
                                    const std::vector<tinyobj::material_t> &materials)
{
    std::cout << "OBJ Model Information:\n";

    // 顶点数
    std::cout << "Number of vertices: " << attrib.vertices.size() / 3 << "\n";

    // 法线数
    std::cout << "Number of normals: " << attrib.normals.size() / 3 << "\n";

    // 纹理坐标数
    std::cout << "Number of texture coordinates: " << attrib.texcoords.size() / 2 << "\n";

    // 打印每个子网格（shape）的信息
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        std::cout << "Shape " << i << " (Name: " << shapes[i].name << "):\n";
        std::cout << "  Number of indices: " << shapes[i].mesh.indices.size() << "\n";
    }

    // 打印材质信息
    std::cout << "Materials:\n";
    for (size_t i = 0; i < materials.size(); ++i)
    {
        std::cout << "  Material " << i << ":\n";
        std::cout << "    Name: " << materials[i].name << "\n";
        std::cout << "    Ambient: (" << materials[i].ambient[0] << ", " << materials[i].ambient[1] << ", "
                  << materials[i].ambient[2] << ")\n";
        std::cout << "    Diffuse: (" << materials[i].diffuse[0] << ", " << materials[i].diffuse[1] << ", "
                  << materials[i].diffuse[2] << ")\n";
        std::cout << "    Specular: (" << materials[i].specular[0] << ", " << materials[i].specular[1] << ", "
                  << materials[i].specular[2] << ")\n";
        std::cout << "    Shininess: " << materials[i].shininess << "\n";
        if (!materials[i].diffuse_texname.empty())
        {
            std::cout << "    Diffuse Texture: " << materials[i].diffuse_texname << "\n";
        }
        if (!materials[i].specular_texname.empty())
        {
            std::cout << "    Specular Texture: " << materials[i].specular_texname << "\n";
        }
    }
}

void ModelLoader::printGltfModelInfo(const tinygltf::Model &model)
{
    std::cout << "GLTF Model Information:\n";

    // 打印网格信息
    std::cout << "Meshes:\n";
    for (size_t i = 0; i < model.meshes.size(); ++i)
    {
        const auto &mesh = model.meshes[i];
        std::cout << "  Mesh " << i << " (Name: " << mesh.name << "):\n";
        std::cout << "    Number of primitives: " << mesh.primitives.size() << "\n";
        for (const auto &primitive : mesh.primitives)
        {
            std::cout << "      Primitive:\n";
            std::cout << "        Mode: " << primitive.mode << "\n"; // 渲染模式，如 TRIANGLES
            std::cout << "        Number of attributes: " << primitive.attributes.size() << "\n";
            for (const auto &attrib : primitive.attributes)
            {
                std::cout << "        Attribute: " << attrib.first << "\n"; // 位置、法线、纹理坐标等
            }
        }
    }

    // 打印材质信息
    std::cout << "Materials:\n";
    for (size_t i = 0; i < model.materials.size(); ++i)
    {
        const auto &material = model.materials[i];
        std::cout << "  Material " << i << " (Name: " << material.name << "):\n";
        std::cout << "    Base color factor: (" << material.pbrMetallicRoughness.baseColorFactor[0] << ", "
                  << material.pbrMetallicRoughness.baseColorFactor[1] << ", "
                  << material.pbrMetallicRoughness.baseColorFactor[2] << ", "
                  << material.pbrMetallicRoughness.baseColorFactor[3] << ")\n";
        std::cout << "    Metallic factor: " << material.pbrMetallicRoughness.metallicFactor << "\n";
        std::cout << "    Roughness factor: " << material.pbrMetallicRoughness.roughnessFactor << "\n";
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
        {
            std::cout << "    Base color texture: "
                      << model.textures[material.pbrMetallicRoughness.baseColorTexture.index].source << "\n";
        }
        if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
        {
            std::cout << "    Metallic-roughness texture: "
                      << model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index].source << "\n";
        }
    }
}

} // namespace Airwave