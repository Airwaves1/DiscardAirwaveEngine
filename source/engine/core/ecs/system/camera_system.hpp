#pragma once

#include "system.hpp"
#include "camera/camera.hpp"
#include "camera/controller/fps_controller.hpp"
#include "camera/controller/trackball_controller.hpp"
#include "ecs/component/camera/camera_controller_component.hpp"

namespace Airwave
{
class CameraSystem : public System
{
  public:
    CameraSystem() {}
    ~CameraSystem() override {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) override
    {
        auto fspControllerView = scene->getRegistry().view<FPSCameraControllerComponent>();
        for (auto entity : fspControllerView)
        {
            auto &controller = fspControllerView.get<FPSCameraControllerComponent>(entity);
            controller.controller->update(deltaTime);
        }

        // auto trackballControllerView =
        // scene->getRegistry().view<TrackballCameraControllerComponent>(); for (auto entity :
        // trackballControllerView)
        // {
        //     auto &controller =
        //     trackballControllerView.get<TrackballCameraControllerComponent>(entity);
        //     controller.controller->rotate(glm::vec2(0.0f, 0.0f));
        // }

        // auto view = scene->getRegistry().view<CameraComponent>();

    }

  private:
};

} // namespace Airwave
