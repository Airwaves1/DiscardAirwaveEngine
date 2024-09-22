#pragma once

#include "render/material/material.hpp"
#include "ecs/component/light/light_component.hpp"
#include "ecs/component/material/material_component.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "system.hpp"

namespace Airwave
{
class LightSystem : public System
{
  public:
    LightSystem()  = default;
    ~LightSystem() = default;

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) {}

    // 更新光源数据
    void updateLights(entt::registry &registry)
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

    void applyLightsToMaterial(const std::shared_ptr<MaterialComponent> &material) const
    {
        auto shader = material->shader;
        if(shader == nullptr)
        {
            LOG_ERROR("Material shader is nullptr");
            return;
        }
        shader->bind();
        shader->uploadUniformInt("u_lightCount", static_cast<int>(m_lights.size()));

        for (size_t i = 0; i < m_lights.size(); i++)
        {
            const auto &light = m_lights[i];
            shader->uploadUniformInt("u_lights[" + std::to_string(i) + "].type", static_cast<int>(light.type));
            shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].intensity", light.intensity);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].ambient", light.ambient);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].diffuse", light.diffuse);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].specular", light.specular);
            if (light.type == LightType::Point)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].position", light.position);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].constant", light.constant);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].linear", light.linear);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].quadratic", light.quadratic);
            }
            else if (light.type == LightType::Directional)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].direction", light.direction);
            }
            else if (light.type == LightType::Spot)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].position", light.position);
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].direction", light.direction);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].constant", light.constant);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].linear", light.linear);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].quadratic", light.quadratic);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].cutOff", light.cutOff);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].outerCutOff", light.outerCutOff);
            }
        }
    }

    // 获取光源数据
    const std::vector<LightComponent> &getLights() const { return m_lights; }

    // 设置最大光源数量
    void setMaxLights(size_t maxLights) { m_maxLights = maxLights; }

  private:
    std::vector<LightComponent> m_lights;
    size_t m_maxLights = 16;
};

} // namespace Airwave
