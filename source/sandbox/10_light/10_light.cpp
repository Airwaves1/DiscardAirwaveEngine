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
        phongMaterial.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        phongMaterial.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        phongMaterial.specular = glm::vec3(0.5f, 0.5f, 0.5f);
        phongMaterial.shininess = 32.0f;
        phongMaterial.diffuseMap = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
        phongMaterial.specularMap = Airwave::Texture2D::Create(TEXTURE_DIR "container2_specular.png");
        
        auto &geometry    = entity->addComponent<Airwave::GeometryComponent>();
        geometry.geometry = std::make_shared<Airwave::CubeGeometry>();

        // 添加光源
        auto lightEntity_0 = m_scene->createAwEntity("light_0");
        auto &pointLight = lightEntity_0->addComponent<Airwave::LightComponent>();
        pointLight.type = Airwave::LightType::Point;
        pointLight.intensity = 1.0f;
        pointLight.position = glm::vec3(3.0f, 0.0f, 3.0f);
        pointLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        pointLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

        // 添加渲染系统
        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }