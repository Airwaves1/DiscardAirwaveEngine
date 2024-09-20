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
        // 点光源
        auto lightEntity_0 = m_scene->createAwEntity("light_0");
        auto &pointLight = lightEntity_0->addComponent<Airwave::LightComponent>();
        pointLight.type = Airwave::LightType::Point;
        pointLight.intensity = 1.0f;
        pointLight.position = glm::vec3(0.0f, 2.0f, 3.0f);
        pointLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        pointLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

        // 可视化点光源
        auto &pointLightCubeMat = lightEntity_0->addComponent<Airwave::BasicMaterialComponent>();
        pointLightCubeMat.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        auto &pointLightGeometry = lightEntity_0->addComponent<Airwave::GeometryComponent>();
        pointLightGeometry.geometry = std::make_shared<Airwave::CubeGeometry>();


        // 平行光
        auto lightEntity_1 = m_scene->createAwEntity("light_1");
        auto &dirLight = lightEntity_1->addComponent<Airwave::LightComponent>();
        dirLight.type = Airwave::LightType::Directional;
        dirLight.intensity = 1.0f;
        dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        dirLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);


        // 聚光灯
        auto lightEntity_2 = m_scene->createAwEntity("light_2");
        auto &spotLight = lightEntity_2->addComponent<Airwave::LightComponent>();
        spotLight.type = Airwave::LightType::Spot;
        spotLight.intensity = 1.0f;
        spotLight.position = glm::vec3(0.0f, 0.0f, 3.0f);
        spotLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        spotLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        spotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight.constant = 1.0f;
        spotLight.linear = 0.09f;
        spotLight.quadratic = 0.032f;
        spotLight.cutOff = glm::cos(glm::radians(12.5f));
        spotLight.outerCutOff = glm::cos(glm::radians(17.5f));




        m_scene->destroyAwEntity(lightEntity_0);
        m_scene->destroyAwEntity(lightEntity_1);


        // 添加渲染系统
        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }