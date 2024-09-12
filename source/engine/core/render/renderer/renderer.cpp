#include "renderer.hpp"
#include "render/renderer/render_command.hpp"

namespace Airwave
{

Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData();

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::BeginScene(std::shared_ptr<Camera> camera)
{
    s_SceneData->ViewProjectionMatrix = camera->getViewMatrix() * camera->getProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &va,
                      const glm::mat4 &transform)
{
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->UploadUniformMat4("u_Model", transform);

    RenderCommand::DrwaIndexed(va);
}
void Renderer::OnViewportResize(uint32_t width, uint32_t height)
{
    RenderCommand::OnViewportResize(width, height);
}
} // namespace Airwave
