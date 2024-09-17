#pragma once

#include "system.hpp"

namespace Airwave
{
class LightSystem : public System
{
  public:
    virtual void update(float deltaTime) override {}

  private:
    void uploadDirectionalLight();
    void uploadPointLight();
    void uploadSpotLight();
};

} // namespace Airwave
