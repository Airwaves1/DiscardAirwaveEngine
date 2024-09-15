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

    void render( const std::shared_ptr<Framebuffer> &framebuffer);

  private:
    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Framebuffer> m_resolveFramebuffer;
    
    };
} // namespace Airwave
