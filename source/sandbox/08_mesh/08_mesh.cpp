#include "airwave_engine.hpp"
#include <glm/glm.hpp>

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
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
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 8.0f));
        m_trackballController = std::make_shared<Airwave::TrackballController>(m_camera);

        // 设置材质和几何体
        m_basicMaterial = std::make_shared<Airwave::BasicMaterial>();
        m_cube          = std::make_shared<Airwave::CubeGeometry>();
    }

    void onUpdate(float deltaTime) override
    {
        // 渲染到帧缓冲
        m_framebuffer->bind();
        Airwave::Renderer::BeginScene(m_camera);
        {
            Airwave::RenderCommand::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
            Airwave::RenderCommand::Clear();

            Airwave::RenderCommand::Enable(Airwave::RenderState::DepthTest);

            Airwave::Renderer::Submit(m_cube, m_basicMaterial, glm::mat4(1.0f));
        }
        Airwave::Renderer::EndScene();
        m_framebuffer->unbind();

        // 渲染全屏四边形
        m_fullScreenQuad->render(m_framebuffer);
    }

  private:
    std::shared_ptr<Airwave::Framebuffer> m_framebuffer;
    std::shared_ptr<Airwave::FullScreenQuad> m_fullScreenQuad;

    std::shared_ptr<Airwave::BasicMaterial> m_basicMaterial;
    std::shared_ptr<Airwave::CubeGeometry> m_cube;

    std::shared_ptr<Airwave::TrackballController> m_trackballController;
    std::shared_ptr<Airwave::Camera> m_camera;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }