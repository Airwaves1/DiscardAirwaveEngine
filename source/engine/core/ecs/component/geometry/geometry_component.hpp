#pragma once

#include "render/geometry/geometry.hpp"

namespace Airwave
{
    struct GeometryComponent
    {
        std::shared_ptr<Geometry> geometry;

        GeometryComponent() {}
    };
} // namespace Airwave