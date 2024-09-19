#pragma once

#include "camera/camera.hpp"
#include <memory>

namespace Airwave
{
    struct CameraComponent
    {
        std::shared_ptr<Camera> camera;
    };
    
} // namespace Airwave
