#pragma once

#include "ecs/system/system.hpp"

namespace Airwave
{

class BackgroundSystem : public System
{
  public:
    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) {}

  private:
};

} // namespace Airwave
