#pragma once
#include "system.hpp"
#include "utils/log.hpp"
#include "camera/controller/fps_controller.hpp"
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"
#include "render/material/material.hpp"
#include "render/geometry/geometry.hpp"
#include "render/shader/shader.hpp"
#include "render/texture/texture.hpp"
#include "render/light/light_manager.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/geometry/geometry_component.hpp"
#include "ecs/component/camera/camera_component.hpp"
#include "ecs/component/light/light_component.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/system/light_system.hpp"
#include "ecs/system/material_system.hpp"
#include "ecs/system/background_system.hpp"
#include "ecs/component/object/mesh_component.hpp"
#include "ecs/system/transform_system.hpp"
#include "event/event_observer.hpp"
#include "render/renderer/render_target.hpp"
#include "render/texture/cube_texture.hpp"
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
        spec.enableMSAA = false;
        spec.samples    = 8;
        m_framebuffer   = Airwave::Framebuffer::Create(1200, 900, spec);

        m_eventObserver = std::make_shared<EventObserver>();
        m_eventObserver->onEvent<WindowResizeEvent>(
            [this](const WindowResizeEvent &e)
            {
                // LOG_DEBUG("WindowResizeEvent: {0},{1}", e.getWindowWidth(), e.getWindowHeight());
                m_framebuffer->resize(e.getWindowWidth(), e.getWindowHeight());
            });

        m_materialSystem = std::make_shared<MaterialSystem>();

        m_renderTarget = std::make_shared<RenderTarget>(1200, 900, TextureSpecification());

    }
    virtual ~ForwardRenderSystem() {}

    void init() {}

    void render() {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) override
    {

        // ########################## 1. 获取相机组件 ##########################
        auto cameraView = scene->getRegistry().view<CameraComponent>();
        if (cameraView.size() == 0)
        {
            LOG_ERROR("No camera entity found!");
            return;
        }

        // 只取第一个相机
        auto entity           = *cameraView.begin();
        auto &cameraComponent = cameraView.get<CameraComponent>(entity);

        // ########################## 2. 变换组件系统 ##########################
        if (!scene->hasSystem<TransformSystem>())
        {
            m_transformSystem = std::make_shared<TransformSystem>();
            scene->addSystem(m_transformSystem);
        }
        else
        {
            m_transformSystem = scene->getSystem<TransformSystem>();
        }

        // ########################## 3. 设置渲染目标 ##########################
        m_framebuffer->bind();
        RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        RenderCommand::Clear();
        RenderCommand::Enable(RenderState::DepthTest);

        // ########################## 4. 获取灯光组件 ##########################
        if (!scene->hasSystem<LightSystem>())
        {
            m_lightSystem = std::make_shared<LightSystem>();
            scene->addSystem(m_lightSystem);
        }
        else
        {
            m_lightSystem = scene->getSystem<LightSystem>();
        }

        m_lightSystem->updateLights(scene->getRegistry());



        // 处理网格组件
        auto meshView = scene->getRegistry().view<MeshComponent, TransformComponent>();
        meshView.each(
            [&](auto entity, MeshComponent &mesh, TransformComponent &transform)
            {
                // 检查几何体组件是否有效，提前返回避免无效绘制
                if (mesh.geometryComponent == nullptr)
                {
                    LOG_ERROR("Geometry component is nullptr for mesh: {0}", mesh.name);
                    return; // 继续处理下一个网格
                }

                // 检查材质组件是否有效，提前返回避免无效绘制
                if (mesh.materialComponent == nullptr)
                {
                    LOG_ERROR("Material component is nullptr for mesh: {0}", mesh.name);
                    return; // 继续处理下一个网格
                }

                // 更新材质信息
                m_materialSystem->updateMaterial(mesh.materialComponent, cameraComponent, transform, deltaTime);

                // 应用光照到材质
                m_lightSystem->applyLightsToMaterial(mesh.materialComponent);

                // 绘制几何体
                mesh.geometryComponent->draw();
            });

                    // ########################## 5. 设置背景 ##########################
        if (!scene->hasSystem<BackgroundSystem>())
        {
            m_backgroundSystem = std::make_shared<BackgroundSystem>(glm::vec4(0.6f, 0.2f, 0.2f, 1.0f));
            scene->addSystem(m_backgroundSystem);
        }
        else
        {
            m_backgroundSystem = scene->getSystem<BackgroundSystem>();
        }

        m_backgroundSystem->render(cameraComponent.camera);

        // ########################## 7. 渲染完成后的处理 ##########################

        Renderer::EndScene();
        m_framebuffer->unbind();

        // 渲染全屏四边形
        m_fullScreenQuad->resolve(m_framebuffer);

        auto texture = m_fullScreenQuad->getFramebuffer()->getColorAttachments()[0];
        m_fullScreenQuad->render(texture, shader);

        m_fullScreenQuad->renderToDefaultFramebuffer();
    }

  private:
    std::string vertexSrc = R"(
        #version 410 core
        layout(location = 0) in vec2 a_position;
        layout(location = 1) in vec2 a_texCoord;

        out vec2 v_texCoord;

        void main()
        {
            gl_Position = vec4(a_position, 0.0, 1.0);
            v_texCoord = a_texCoord;
        }
    )";

    std::string fragmentSrc        = R"(
        #version 410 core
        layout(location = 0) out vec4 color;

        in vec2 v_texCoord;

        uniform sampler2D u_depthTexture;
        uniform sampler2D u_texture;

        void main()
        {
            // color = vec4(v_texCoord, 0.0, 1.0);

            float depth = texture(u_depthTexture, v_texCoord).r;
            color = texture(u_texture, v_texCoord);
            color = vec4(color.rgb, 1.0);
            // color = vec4(vec3(depth), 1.0);
            // color = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
        }
    )";
    std::shared_ptr<Shader> shader = SHADER_LIB.load("test", vertexSrc, fragmentSrc, false);

    std::shared_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<FullScreenQuad> m_fullScreenQuad;

    std::shared_ptr<RenderTarget> m_renderTarget;

    std::shared_ptr<LightSystem> m_lightSystem;
    std::shared_ptr<MaterialSystem> m_materialSystem;
    std::shared_ptr<TransformSystem> m_transformSystem;
    std::shared_ptr<BackgroundSystem> m_backgroundSystem;

    std::shared_ptr<EventObserver> m_eventObserver;

};

} // namespace Airwave
