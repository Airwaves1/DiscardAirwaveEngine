#pragma once
#include "system.hpp"
#include "utils/log.hpp"
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"
#include "render/material/material.hpp"
#include "render/geometry/geometry.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture.hpp"
#include "render/light/light_manager.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/geometry/geometry_component.hpp"
#include "ecs/component/light/light_component.hpp"
namespace Airwave
{
class ForwardRenderSystem : public System
{
  public:
    ForwardRenderSystem()
    {
        // 设置帧缓冲和全屏渲染
        m_fullScreenQuad = std::make_shared<Airwave::FullScreenQuad>();

        Airwave::FramebufferSpecification spec;
        spec.enableMSAA = true;
        spec.samples    = 4;
        m_framebuffer   = Airwave::Framebuffer::Create(1200, 900, spec);

        // 设置相机和控制器
        m_camera = std::make_shared<Airwave::PerspectiveCamera>(
            45.0f, static_cast<float>(1200) / 900, 0.1f, 100.0f);
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
        m_trackballController = std::make_shared<Airwave::TrackballController>(m_camera);
    }
    virtual ~ForwardRenderSystem() {}

    void init() {}

    void render() {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) override
    {
        m_framebuffer->bind();
        Renderer::BeginScene(m_camera);
        RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        RenderCommand::Clear();
        RenderCommand::Enable(RenderState::DepthTest);

        auto view = scene->getRegistry().view<PhongMaterialComponent, GeometryComponent>();
        auto lightView = scene->getRegistry().view<PointLightComponent>();

        view.each(
            [&](auto entity, PhongMaterialComponent &material, GeometryComponent &geometry)
            {

                lightView.each([&](auto entity, PointLightComponent &light)
                {
                    material.material->setUniform("u_lightColor", light.light->color);
                });
                material.material->setUniform("u_color", material.color);
                Renderer::Submit(geometry.geometry, material.material, glm::mat4(1.0f));
            });

        Renderer::EndScene();
        m_framebuffer->unbind();
        // 渲染全屏四边形
        m_fullScreenQuad->render(m_framebuffer);
    }

  private:
    std::shared_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<FullScreenQuad> m_fullScreenQuad;
    std::shared_ptr<TrackballController> m_trackballController;
    std::shared_ptr<Camera> m_camera;
    LightManager m_lightManager;
};

} // namespace Airwave