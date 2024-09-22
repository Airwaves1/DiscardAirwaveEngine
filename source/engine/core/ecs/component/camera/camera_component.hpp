#pragma once

#include "camera/camera.hpp"
#include "ecs/aw_component.hpp"
#include <memory>

namespace Airwave
{
    struct CameraComponent
    {
        std::shared_ptr<Camera> camera;
    };
    
} // namespace Airwave
