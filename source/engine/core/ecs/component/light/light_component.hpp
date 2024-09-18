#pragma once
#include "render/light/light.hpp"
#include "render/light/light_manager.hpp"

namespace Airwave
{
struct PointLightComponent
{
    std::shared_ptr<PointLight> light;
    
    PointLightComponent() { light = std::make_shared<PointLight>(); }
};

} // namespace Airwave