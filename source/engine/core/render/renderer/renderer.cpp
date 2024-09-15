#include "renderer.hpp"
#include "render/renderer/render_command.hpp"

namespace Airwave
{

Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData();

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::BeginScene(std::shared_ptr<Camera> camera)
{
    s_SceneData->ViewMatrix = camera->getViewMatrix();
    s_SceneData->ProjectionMatrix = camera->getProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &va,
                      const glm::mat4 &transform)
{
    shader->bind();
        shader->uploadUniformMat4("u_viewMatrix", s_SceneData->ViewMatrix);
        shader->uploadUniformMat4("u_projectionMatrix", s_SceneData->ProjectionMatrix);
        shader->uploadUniformMat4("u_modelMatrix", transform);

    RenderCommand::DrwaIndexed(va);
}
void Renderer::OnViewportResize(uint32_t width, uint32_t height)
{
    RenderCommand::OnViewportResize(width, height);
}
} // namespace Airwave
