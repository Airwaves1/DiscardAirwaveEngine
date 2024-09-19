#pragma once
#include <glm/glm.hpp>
#include "render/texture/texture_2d.hpp"
#include "render/shader/shader.hpp"
namespace Airwave
{
enum class LightType
{
    Point,
    Directional,
    Spot
};

struct LightComponent
{
    LightType type;  // 光源类型
    float intensity; // 光源强度（在PBR中可表示辐射率）

    // 光照分量
    glm::vec3 ambient;  // 环境光颜色
    glm::vec3 diffuse;  // 漫反射颜色
    glm::vec3 specular; // 镜面反射颜色

    glm::vec3 position;  // 光源位置（对点光源和聚光灯有效）
    glm::vec3 direction; // 光源方向（对平行光和聚光灯有效）

    float constant;   // 衰减系数（点光源和聚光灯使用）
    float shadowBias; // 阴影偏移
    bool castsShadow; // 是否产生阴影

    // 纹理贴图
    std::shared_ptr<Texture2D> shadowMap;      // 阴影贴图
    std::shared_ptr<Texture2D> irradianceMap;  // 漫反射环境光贴图（用于PBR）
    std::shared_ptr<Texture2D> environmentMap; // 环境贴图（用于PBR）

    LightComponent(LightType type = {}, float intensity = {},
                   bool castsShadow = false)
        : type(type), intensity(intensity), shadowBias(0.005f),
          castsShadow(castsShadow)
    {
        // 默认光源位置和方向
        position  = glm::vec3(0.0f, 1.0f, 0.0f);
        direction = glm::vec3(0.0f, -1.0f, 0.0f);
    }
};

} // namespace Airwave