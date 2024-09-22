#include "airwave_engine.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {

        // 创建场景
        m_scene = std::make_shared<Airwave::Scene>("sandbox");

        // 添加场景渲染实体
        auto entity = m_scene->createAwEntity("phong cube");
        // 添加材质组件
        Airwave::MaterialComponent material = {
            .type        = Airwave::MaterialType::BlinnPhong,
            .diffuse       = glm::vec3(1.0f, 0.5f, 0.31f),
            .ambient     = glm::vec3(1.0f),
            .specular    = glm::vec3(0.5f),
            .shininess   = 32.0f,
            .diffuseMap  = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png"),
            .specularMap = Airwave::Texture2D::Create(TEXTURE_DIR "container2_specular.png"),
        };
        entity->addComponent<Airwave::MaterialComponent>(material);

        // 设置几何形状
        auto &geometry = entity->addComponent<Airwave::GeometryComponent>();
        std::vector<Airwave::AWVertex> vertices;
        std::vector<uint32_t> indices;
        Airwave::GeometryUtils::CreateCube(vertices, indices);
        geometry.setData(vertices, indices);

        // 设置变换组件
        auto &transform = entity->addComponent<Airwave::TransformComponent>();

        auto entity_1                        = m_scene->createAwEntity("phong cube_1");
        Airwave::MaterialComponent material2 = {
            .type        = Airwave::MaterialType::BlinnPhong,
            .diffuse       = glm::vec3(1.0f, 1.0f, 1.0f),
            .ambient     = glm::vec3(1.0f),
            .specular    = glm::vec3(1.0f),
            .shininess   = 32.0f,
            .diffuseMap  = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg"),
            .specularMap = Airwave::Texture2D::Create(TEXTURE_DIR "container2_specular.png"),
        };
        entity_1->addComponent<Airwave::MaterialComponent>(material2);
        auto &geometry_1 = entity_1->addComponent<Airwave::GeometryComponent>();
        geometry_1.setData(vertices, indices);
        auto &transform_1 = entity_1->addComponent<Airwave::TransformComponent>();
        transform_1.setPosition(glm::vec3(1.5f, 0.0f, 0.0f));

        // 添加光源
        // 点光源
        auto lightEntity_0   = m_scene->createAwEntity("light_0");
        auto &pointLight     = lightEntity_0->addComponent<Airwave::LightComponent>();
        pointLight.type      = Airwave::LightType::Point;
        pointLight.intensity = 1.0f;
        pointLight.position  = glm::vec3(0.0f, 2.0f, 3.0f);
        pointLight.ambient   = glm::vec3(0.2f, 0.2f, 0.2f);
        pointLight.diffuse   = glm::vec3(1.0f, 1.0f, 1.0f);
        pointLight.specular  = glm::vec3(1.0f, 1.0f, 1.0f);

        // 添加渲染系统
        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);

        // 添加相机系统
        auto cameraSystem = std::make_shared<Airwave::CameraSystem>();
        m_scene->addSystem(cameraSystem);

        // 创建相机实体
        auto cameraEntity = m_scene->createAwEntity("main camera");
        // 添加相机组件
        auto &cameraComponent = cameraEntity->addComponent<Airwave::CameraComponent>();
        cameraComponent.camera =
            std::make_shared<Airwave::PerspectiveCamera>(45.0f, Application::getAspectRatio(), 0.1f, 100.0f);
        cameraComponent.camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        // 添加相机控制器组件
        auto &cameraController      = cameraEntity->addComponent<Airwave::FPSCameraControllerComponent>();
        cameraController.controller = std::make_shared<Airwave::FPSCameraController>(cameraComponent.camera);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Entity Settings");

        // // 获取 "phong cube" 实体
        if (auto entity = m_scene->getAwEntity("phong cube"); entity)
        {
            // 获取变换组件
            auto &transform = entity->getComponent<Airwave::TransformComponent>();

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // 分隔线
                ImGui::Separator();
                ImGui::Text("Position");

                // 显示并编辑位置
                glm::vec3 position = transform.getPosition();
                if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.01f))
                {
                    transform.setPosition(position); // 更新位置
                }

                ImGui::Separator();
                ImGui::Text("Rotation");

                // 显示并编辑旋转（转换为欧拉角以便编辑）
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(transform.getRotation()));
                if (ImGui::DragFloat3("##Rotation", glm::value_ptr(eulerRotation), 0.5f))
                {
                    glm::quat newRotation = glm::quat(glm::radians(eulerRotation));
                    transform.setRotation(newRotation); // 更新旋转
                }

                ImGui::Separator();
                ImGui::Text("Scale");

                // 显示并编辑缩放
                glm::vec3 scale = transform.getScale();
                if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.01f, 0.1f, 10.0f))
                {
                    transform.setScale(scale); // 更新缩放
                }
            }
        }

        ImGui::End();
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }