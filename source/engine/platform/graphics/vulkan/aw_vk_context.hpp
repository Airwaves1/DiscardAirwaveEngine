#pragma once

#include "graphics/graphic_contex.hpp"

namespace Airwave
{
class AwVkContext : public GraphicContext
{
  public:
    AwVkContext() = delete;
    AwVkContext(AwWindow *window);
    ~AwVkContext() override;

  private:
    void createInstance();
};

} // namespace Airwave
