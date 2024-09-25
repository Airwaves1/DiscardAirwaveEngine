#pragma once

#include "render/buffer/framebuffer.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
class RenderPass
{
  public:
    RenderPass() = default;

    virtual ~RenderPass() = default;

    virtual void execute(const std::shared_ptr<Framebuffer>& framebuffer) = 0;
    
    virtual std::shared_ptr<Texture2D> getOutputTexture() = 0;

    virtual void setInputTexture(const std::shared_ptr<Texture2D>& texture) = 0;
};

} // namespace Airwave
