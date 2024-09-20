#pragma once

#include "render/material/material.hpp"
#include "ecs/component/light/light_component.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "system.hpp"

namespace Airwave
{
class LightSystem : public System
{
  public:
    LightSystem() = default;
    ~LightSystem() = default;

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) {}

    // 更新光源数据
    void updateLights(entt::registry& registry)
    {
        m_lights.clear();

        auto lightView = registry.view<LightComponent>();

        lightView.each(
            [&](auto entity, LightComponent &light)
            {
                if (m_lights.size() < m_maxLights)
                {
                    m_lights.push_back(light);
                }
            });
    }

    void applyLightsToMaterial(std::shared_ptr<Material> material)
    {
        material->setUniform("u_lightCount", static_cast<int>(m_lights.size()));

        for (size_t i = 0; i < m_lights.size(); i++)
        {
            const auto &light = m_lights[i];
            material->setUniform("u_lights[" + std::to_string(i) + "].type", static_cast<int>(light.type));
            material->setUniform("u_lights[" + std::to_string(i) + "].intensity", light.intensity);
            material->setUniform("u_lights[" + std::to_string(i) + "].ambient", light.ambient);
            material->setUniform("u_lights[" + std::to_string(i) + "].diffuse", light.diffuse);
            material->setUniform("u_lights[" + std::to_string(i) + "].specular", light.specular);
            if(light.type == LightType::Point){
                material->setUniform("u_lights[" + std::to_string(i) + "].position", light.position);
                material->setUniform("u_lights[" + std::to_string(i) + "].constant", light.constant);
                material->setUniform("u_lights[" + std::to_string(i) + "].linear", light.linear);
                material->setUniform("u_lights[" + std::to_string(i) + "].quadratic", light.quadratic);
            }else if(light.type == LightType::Directional){
                material->setUniform("u_lights[" + std::to_string(i) + "].direction", light.direction);
            }else if(light.type == LightType::Spot){
                material->setUniform("u_lights[" + std::to_string(i) + "].position", light.position);
                material->setUniform("u_lights[" + std::to_string(i) + "].direction", light.direction);
                material->setUniform("u_lights[" + std::to_string(i) + "].constant", light.constant);
                material->setUniform("u_lights[" + std::to_string(i) + "].linear", light.linear);
                material->setUniform("u_lights[" + std::to_string(i) + "].quadratic", light.quadratic);
                material->setUniform("u_lights[" + std::to_string(i) + "].cutOff", light.cutOff);
                material->setUniform("u_lights[" + std::to_string(i) + "].outerCutOff", light.outerCutOff);
            }
        }
    }

    // 获取光源数据
    const std::vector<LightComponent>& getLights() const
    {
        return m_lights;
    }

    // 设置最大光源数量
    void setMaxLights(size_t maxLights) { m_maxLights = maxLights; }

  private:
    std::vector<LightComponent> m_lights;
    size_t m_maxLights = 16;
};

} // namespace Airwave
