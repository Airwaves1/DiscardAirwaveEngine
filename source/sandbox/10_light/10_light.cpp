#include "airwave_engine.hpp"
#include <glm/glm.hpp>

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {
        // 创建场景
        m_scene     = std::make_shared<Airwave::Scene>("sandbox");



        // 添加场景渲染实体
        auto entity = m_scene->createAwEntity("phong cube");
        auto &phongMaterial = entity->addComponent<Airwave::PhongMaterialComponent>();
        phongMaterial.color = glm::vec4(0.8f, 0.4f, 0.3f, 1.0f);
        auto &geometry    = entity->addComponent<Airwave::GeometryComponent>();
        geometry.geometry = std::make_shared<Airwave::CubeGeometry>();

        // 添加光源
        auto pointLightEntity = m_scene->createAwEntity("point light");
        auto &pointLight = pointLightEntity->addComponent<Airwave::PointLightComponent>();
        pointLight.light->color = glm::vec3(1.0f, 1.0f, 1.0f);
        pointLight.light->intensity = glm::vec3(1.0f, 1.0f, 1.0f);
        pointLight.light->position = glm::vec3(3.0f, 3.0f, 3.0f);

        // 添加渲染系统
        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }