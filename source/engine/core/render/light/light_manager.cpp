#include "light_manager.hpp"

namespace Airwave
{

void LightManager::uploadLightsToShader(std::shared_ptr<Shader> shader)
{
    // 上传方向光
    for (int i = 0; i < m_directionalLights.size(); i++)
    {
        shader->uploadUniformFloat3("u_directionalLights[" + std::to_string(i) + "].direction",
                                    m_directionalLights[i]->direction);
        shader->uploadUniformFloat3("u_directionalLights[" + std::to_string(i) + "].intensity",
                                    m_directionalLights[i]->intensity);
        shader->uploadUniformFloat3("u_directionalLights[" + std::to_string(i) + "].color",
                                    m_directionalLights[i]->color);
    }

    // 上传点光源
    for (int i = 0; i < m_pointLights.size(); i++)
    {
        shader->uploadUniformFloat3("u_pointLights[" + std::to_string(i) + "].position",
                                    m_pointLights[i]->position);
        shader->uploadUniformFloat3("u_pointLights[" + std::to_string(i) + "].intensity",
                                    m_pointLights[i]->intensity);
        shader->uploadUniformFloat3("u_pointLights[" + std::to_string(i) + "].color",
                                    m_pointLights[i]->color);
        // shader->uploadUniformFloat("u_pointLights[" + std::to_string(i) + "].constant",
        //                            m_pointLights[i]->constant);
        // shader->uploadUniformFloat("u_pointLights[" + std::to_string(i) + "].linear",
        //                            m_pointLights[i]->linear);
        // shader->uploadUniformFloat("u_pointLights[" + std::to_string(i) + "].quadratic",
        //                            m_pointLights[i]->quadratic);
    }

    // 上传聚光灯
    for (int i = 0; i < m_spotLights.size(); i++)
    {
        shader->uploadUniformFloat3("u_spotLights[" + std::to_string(i) + "].position",
                                    m_spotLights[i]->position);
        shader->uploadUniformFloat3("u_spotLights[" + std::to_string(i) + "].direction",
                                    m_spotLights[i]->direction);
        shader->uploadUniformFloat3("u_spotLights[" + std::to_string(i) + "].intensity",
                                    m_spotLights[i]->intensity);
        shader->uploadUniformFloat3("u_spotLights[" + std::to_string(i) + "].color",
                                    m_spotLights[i]->color);
        shader->uploadUniformFloat("u_spotLights[" + std::to_string(i) + "].constant",
                                   m_spotLights[i]->constant);
        shader->uploadUniformFloat("u_spotLights[" + std::to_string(i) + "].linear",
                                   m_spotLights[i]->linear);
        shader->uploadUniformFloat("u_spotLights[" + std::to_string(i) + "].quadratic",
                                   m_spotLights[i]->quadratic);
    }
}

} // namespace Airwave