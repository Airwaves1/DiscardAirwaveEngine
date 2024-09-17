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
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 50.0f));
        m_trackballController = std::make_shared<Airwave::TrackballController>(m_camera);
    
        // 加载纹理
        // m_texture_0 = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
        // m_texture_1 = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg");
        auto texture0 = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
        auto texture1 = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg");
        auto texture2 = Airwave::Texture2D::Create(TEXTURE_DIR "awesomeface.png");
        m_mesh =std::make_shared<Airwave::Mesh>(std::make_shared<Airwave::CubeGeometry>(), std::make_shared<Airwave::BasicMaterial>());
        // m_mesh =std::make_shared<Airwave::Mesh>(std::make_shared<Airwave::SphereGeometry>(0.5), std::make_shared<Airwave::BasicMaterial>());
        m_mesh->getMaterial()->setUniform("u_color", glm::vec4(0.8f, 0.5f, 0.6f, 0.3f));
        // m_mesh->getMaterial()->setTexture("u_textures[0]", texture0,0);
        // m_mesh->getMaterial()->setTexture("u_textures[1]", texture1,1);
        // m_mesh->getMaterial()->setTexture("u_textures[2]", texture2,2);
        // m_mesh->getMaterial()->setUniform("u_blendFactors[0]", 0.7f);
        // m_mesh->getMaterial()->setUniform("u_blendFactors[1]", 0.3f);
        // m_mesh->getMaterial()->setUniform("u_blendFactors[2]", 0.5f);


        auto uuid = Airwave::UUID::Generate();
        LOG_INFO("UUID: {0}", uuid.toString());
    }

    void onUpdate(float deltaTime) override
    {
        // 渲染到帧缓冲
        m_framebuffer->bind();
        Airwave::Renderer::BeginScene(m_camera);
        {
            Airwave::RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
            Airwave::RenderCommand::Clear();
            Airwave::RenderCommand::Enable(Airwave::RenderState::DepthTest);
            for(int i = -5; i < 5; i++)
            {
                for(int j = -5; j < 5; j++)
                {
                    for(int k = -5; k < 5; k++)
                    {
                        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i*2.0f, j*2.0f, k*2.0f));
                        m_mesh->getMaterial()->setUniform("u_color", glm::vec4( (i+5)/10.0f, (j+5)/10.0f, (k+5)/10.0f, 0.3f));
                        Airwave::Renderer::Submit(m_mesh->getGeometry(), m_mesh->getMaterial(), model);
                    }
                }
            }
        }
        Airwave::Renderer::EndScene();
        m_framebuffer->unbind();

        // 渲染全屏四边形
        m_fullScreenQuad->render(m_framebuffer);
    }

  private:
    std::shared_ptr<Airwave::Framebuffer> m_framebuffer;
    std::shared_ptr<Airwave::FullScreenQuad> m_fullScreenQuad;

    std::shared_ptr<Airwave::Mesh> m_mesh;

    std::shared_ptr<Airwave::TrackballController> m_trackballController;
    std::shared_ptr<Airwave::Camera> m_camera;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }