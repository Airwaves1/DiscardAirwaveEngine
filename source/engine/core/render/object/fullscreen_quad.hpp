#pragma once

#include <memory>
#include "utils/common.hpp"
#include "render/buffer/vertex_array.hpp"
#include "render/buffer/vertex_buffer.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture_2d.hpp"
#include "render/buffer/framebuffer.hpp"
namespace Airwave
{
class FullScreenQuad
{
  public:
    FullScreenQuad();
    ~FullScreenQuad() = default;

    void resolve(const std::shared_ptr<Framebuffer> &framebuffer);
    void render(const std::shared_ptr<Texture2D> &texture,const std::shared_ptr<Shader> &shader);
    void renderToDefaultFramebuffer();

    std::shared_ptr<Framebuffer> getFramebuffer() { return m_resolveFramebuffer; }

  private:
    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Framebuffer> m_resolveFramebuffer;

  public:
    std::shared_ptr<Shader> m_shader;
    std::string vertexShader;
    std::string fragmentShader;
};
} // namespace Airwave
