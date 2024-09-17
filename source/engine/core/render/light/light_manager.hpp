#pragma once

#include "light.hpp"
#include <memory>
#include "render/shader/shader.hpp"

namespace Airwave
{
class LightManager
{
  public:
    void addLight(std::shared_ptr<Light> light)
    {
        switch (light->getType())
        {
            case LightType::Directional:
                m_directionalLights.push_back(std::static_pointer_cast<DirectionalLight>(light));
                break;
            case LightType::Point:
                m_pointLights.push_back(std::static_pointer_cast<PointLight>(light));
                break;
            case LightType::Spot:
                m_spotLights.push_back(std::static_pointer_cast<SpotLight>(light));
                break;
        }
    }
    void removeLight(std::shared_ptr<Light> light)
    {
        switch (light->getType())
        {
            case LightType::Directional:
                m_directionalLights.erase(
                    std::remove(m_directionalLights.begin(), m_directionalLights.end(), light),
                    m_directionalLights.end());
                break;
            case LightType::Point:
                m_pointLights.erase(std::remove(m_pointLights.begin(), m_pointLights.end(), light),
                                    m_pointLights.end());
                break;
            case LightType::Spot:
                m_spotLights.erase(std::remove(m_spotLights.begin(), m_spotLights.end(), light),
                                   m_spotLights.end());
                break;
        }
    }

    void uploadLightsToShader(std::shared_ptr<Shader> shader);

  private:
    std::vector<std::shared_ptr<DirectionalLight>> m_directionalLights = {};
    std::vector<std::shared_ptr<PointLight>> m_pointLights             = {};
    std::vector<std::shared_ptr<SpotLight>> m_spotLights               = {};
};

} // namespace Airwave
