#include "airwave_engine.hpp"
#include <glm/glm.hpp>

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {
        m_scene     = std::make_shared<Airwave::Scene>("sandbox");
        auto entity = m_scene->createAwEntity("entity");

        auto &basicMaterial = entity->addComponent<Airwave::BasicMaterialComponent>();
        basicMaterial.material = std::make_shared<Airwave::BasicMaterial>();
        basicMaterial.color = glm::vec4(0.4f, 0.3f, 0.5f, 1.0f);

        auto &geometry    = entity->addComponent<Airwave::GeometryComponent>();
        geometry.geometry = std::make_shared<Airwave::CubeGeometry>();

        auto forwardRenderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(forwardRenderSystem);
    }

    void onUpdate(float deltaTime) override { m_scene->updateSystems(deltaTime); }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }