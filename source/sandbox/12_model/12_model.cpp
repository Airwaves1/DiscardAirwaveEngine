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

        auto model_entity_1 = m_scene->createAwEntity("model_1");
        auto modelComponent = model_entity_1->addComponent<Airwave::ModelComponent>();
        modelComponent->load(MODEL_DIR "nanosuit/nanosuit.obj", model_entity_1);
        // modelComponent->load(MODEL_DIR "intergalactic_spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj",
        //                      model_entity_1);
        auto model_entity_1_transform = model_entity_1->addComponent<Airwave::TransformComponent>();
        model_entity_1_transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        auto model_entity_2   = m_scene->createAwEntity("model_2");
        auto modelComponent_2 = model_entity_2->addComponent<Airwave::ModelComponent>();
        modelComponent_2->load(MODEL_DIR "intergalactic_spaceship-obj/Intergalactic_Spaceship-(Wavefront).obj",
                               model_entity_2);
        auto model_entity_2_transform = model_entity_2->addComponent<Airwave::TransformComponent>();
        model_entity_2_transform->setPosition(glm::vec3(0.0f, 0.0f, 20.0f));

        // 添加光源
        // 点光源
        auto lightEntity_0    = m_scene->createAwEntity("light_0");
        auto pointLight       = lightEntity_0->addComponent<Airwave::LightComponent>();
        pointLight->type      = Airwave::LightType::Point;
        pointLight->intensity = 1.0f;
        pointLight->position  = glm::vec3(0.0f, 2.0f, 3.0f);
        pointLight->ambient   = glm::vec3(0.2f, 0.2f, 0.2f);
        pointLight->diffuse   = glm::vec3(1.0f, 1.0f, 1.0f);
        pointLight->specular  = glm::vec3(1.0f, 1.0f, 1.0f);

        // 方向光
        auto lightEntity_1        = m_scene->createAwEntity("light_1");
        auto directionLight       = lightEntity_1->addComponent<Airwave::LightComponent>();
        directionLight->type      = Airwave::LightType::Directional;
        directionLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        directionLight->ambient   = glm::vec3(0.2f, 0.2f, 0.2f);
        directionLight->diffuse   = glm::vec3(1.0f, 1.0f, 1.0f);
        directionLight->specular  = glm::vec3(1.0f, 1.0f, 1.0f);

        // 聚光灯
        auto lightEntity_2     = m_scene->createAwEntity("light_2");
        auto spotLight         = lightEntity_2->addComponent<Airwave::LightComponent>();
        spotLight->type        = Airwave::LightType::Spot;
        spotLight->position    = glm::vec3(0.0f, 2.0f, 3.0f);
        spotLight->direction   = glm::vec3(0.0f, -1.0f, 0.0f);
        spotLight->cutOff      = glm::cos(glm::radians(12.5f));
        spotLight->outerCutOff = glm::cos(glm::radians(17.5f));
        spotLight->constant    = 1.0f;
        spotLight->linear      = 0.09f;
        spotLight->quadratic   = 0.032f;
        spotLight->ambient     = glm::vec3(0.2f, 0.2f, 0.2f);
        spotLight->diffuse     = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight->specular    = glm::vec3(1.0f, 1.0f, 1.0f);

        // 添加渲染系统
        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);

        // 添加相机系统
        auto cameraSystem = std::make_shared<Airwave::CameraSystem>();
        m_scene->addSystem(cameraSystem);

        // 创建相机实体
        auto cameraEntity = m_scene->createAwEntity("main camera");
        // 添加相机组件
        auto cameraComponent = cameraEntity->addComponent<Airwave::CameraComponent>();
        cameraComponent->camera =
            std::make_shared<Airwave::PerspectiveCamera>(45.0f, Application::getAspectRatio(), 0.1f, 100.0f);
        cameraComponent->camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        // 添加相机控制器组件
        auto cameraController        = cameraEntity->addComponent<Airwave::FPSCameraControllerComponent>();
        cameraController->controller = std::make_shared<Airwave::FPSCameraController>(cameraComponent->camera);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Entity Settings");

        // 获取 "model_1" 实体
        if (auto entity = m_scene->getAwEntity("model_1"); entity)
        {
            // 获取变换组件
            auto transform = entity->getComponent<Airwave::TransformComponent>();
            if (!transform)
            {
                ImGui::Text("Transform component not found.");
                ImGui::End();
                return;
            }

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // 分隔线
                ImGui::Separator();
                ImGui::Text("Position");

                // 显示并编辑位置
                glm::vec3 position = transform->getPosition();
                if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.01f))
                {
                    transform->setPosition(position); // 更新位置
                }

                ImGui::Separator();
                ImGui::Text("Rotation");

                // 显示并编辑旋转（转换为欧拉角以便编辑）
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(transform->getRotation()));
                if (ImGui::DragFloat3("##Rotation", glm::value_ptr(eulerRotation), 0.5f))
                {
                    transform->setRotation(glm::quat(glm::radians(eulerRotation))); // 更新旋转
                }

                ImGui::Separator();
                ImGui::Text("Scale");

                // 显示并编辑缩放
                glm::vec3 scale = transform->getScale();
                if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.01f, 0.1f, 10.0f))
                {
                    transform->setScale(scale); // 更新缩放
                }
            }
        }
        else
        {
            ImGui::Text("Entity 'model_1' not found.");
        }

        ImGui::End();
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
    std::shared_ptr<Airwave::ModelLoader> m_modelLoader;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }