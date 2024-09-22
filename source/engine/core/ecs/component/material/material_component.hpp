#pragma once
#include <glm/glm.hpp>
#include "render/material/material.hpp"
#include "render/material/basic_material.hpp"
#include "render/texture/texture_2d.hpp"
#include "render/shader/shader.hpp"
#include "ecs/aw_component.hpp"

namespace Airwave
{

enum class MaterialType
{
    Standard,
    Unlit,
    BlinnPhong,
    Custom
};

struct MaterialComponent
{
    std::string name = "material";

    MaterialType type = MaterialType::BlinnPhong;

    bool doubleSided = false;

    float alpha = 1.0f;

    glm::vec3 diffuse    = glm::vec3(0.5f); // diffuse
    glm::vec3 ambient  = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
    float shininess    = 32.0f;

    // PBR 特性
    float metallic  = 0.0f; // 金属度
    float roughness = 1.0f; // 粗糙度

    // 纹理
    std::shared_ptr<Texture2D> diffuseMap;
    std::shared_ptr<Texture2D> specularMap;
    std::shared_ptr<Texture2D> normalMap;
    std::shared_ptr<Texture2D> metallicMap;  // PBR
    std::shared_ptr<Texture2D> roughnessMap; // PBR
    std::shared_ptr<Texture2D> emissionMap;  // 自发光

    std::shared_ptr<Shader> shader; // 更改着色器后需要将needUpdate设置为true

    bool specularHighlight = true; // 是否开启高光
    bool needUpdate = false;
};

} // namespace Airwave