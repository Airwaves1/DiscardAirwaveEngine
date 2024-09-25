#pragma once

#include "render/texture/texture_2d.hpp"
#include "render/renderer/render_pass.hpp"
#include <memory>
#include <vector>

namespace Airwave
{
class RenderPipeline
{
  public:
    RenderPipeline() = default;

    void addRenderPass(std::shared_ptr<RenderPass> renderPass) { m_renderPasses.push_back(renderPass); }

    void executePasses(const std::shared_ptr<Framebuffer> &framebuffer)
    {
        std::shared_ptr<Texture2D> inputTexture = nullptr;

        for (auto &renderPass : m_renderPasses)
        {
            if (inputTexture)
            {
                renderPass->setInputTexture(inputTexture);
            }
            renderPass->execute(framebuffer);
            inputTexture = renderPass->getOutputTexture();
        }
    }

  private:
    std::vector<std::shared_ptr<RenderPass>> m_renderPasses;
};

} // namespace Airwave
