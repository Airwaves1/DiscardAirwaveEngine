#pragma once

#include "camera/camera.hpp"
#include "camera/controller/trackball_controller.hpp"
#include "camera/controller/fps_controller.hpp"
#include "ecs/aw_component.hpp"
namespace Airwave
{
class FPSCameraControllerComponent
{
  public:
    std::shared_ptr<FPSCameraController> controller;
};

class TrackballCameraControllerComponent
{
  public:
    std::shared_ptr<TrackballController> controller;
};

} // namespace Airwave
