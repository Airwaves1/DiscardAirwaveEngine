#pragma once
#include "ecs/scene.hpp"

namespace Airwave
{
class System
{
  public:
    System() {}
    virtual ~System() {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) = 0;
};

} // namespace Airwave
