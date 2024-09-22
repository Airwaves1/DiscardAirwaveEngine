#include "render_command.hpp"
#include "utils/common.hpp"
#include "graphics/opengl/gl_renderer_api.hpp"

namespace Airwave
{
RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

void RenderCommand::Init() { s_RendererAPI->init(); }

// 调用此API之前, 绑定了什么Shader, 就用对应的Shader绘制这个vertex array
void RenderCommand::DrwaIndexed(const std::shared_ptr<VertexArray> &va, uint32_t count)
{
    va->bind();
    if (count == 0)
    {
        uint32_t c = va->getIndexBuffer()->getCount();
        s_RendererAPI->drawIndexed(va, c);
    }
    else
        s_RendererAPI->drawIndexed(va, count);
}

void RenderCommand::Clear(RenderClearFlag flag) { s_RendererAPI->clear(flag); }

void RenderCommand::SetClearColor(const glm::vec4 &color) { s_RendererAPI->setClearColor(color); }

void RenderCommand::SetPolygonMode(RenderPolygonMode mode) { s_RendererAPI->setPolygonMode(mode); }

void RenderCommand::Enable(RenderState state) { s_RendererAPI->enable(state); }

void RenderCommand::Disable(RenderState state) { s_RendererAPI->disable(state); }

void RenderCommand::OnViewportResize(uint32_t width, uint32_t height)
{
    s_RendererAPI->onViewportResize(width, height);
}
} // namespace Airwave
