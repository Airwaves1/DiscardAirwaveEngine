#pragma once

#include "ecs/aw_entity.hpp"

namespace Airwave
{

struct AwComponent
{
  public:
    void setOwner(std::shared_ptr<AwEntity> entity) { owner = entity; }
    std::shared_ptr<AwEntity> getOwner() { return owner; }

  private:
    std::shared_ptr<AwEntity> owner;
};

} // namespace Airwave
