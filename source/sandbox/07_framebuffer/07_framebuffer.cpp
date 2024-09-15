#include "airwave_engine.hpp"
#include <glm/glm.hpp>
#include "render/object/fullscreen_quad.hpp"

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {
        m_fullScreenQuad = std::make_shared<Airwave::FullScreenQuad>();
        Airwave::FramebufferSpecification spec;
        spec.enableMSAA = true;
        spec.samples = 4;
        m_framebuffer = Airwave::Framebuffer::Create(1200, 900, spec);

        m_camera = std::make_shared<Airwave::PerspectiveCamera>(
            45.0f, static_cast<float>(1200) / 900, 0.1f, 100.0f);
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 8.0f));
        m_trackballController = std::make_shared<Airwave::TrackballController>(m_camera);

        m_texture_0 = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
        m_texture_1 = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg");

        m_shaderLibrary = std::make_shared<Airwave::ShaderLibrary>();
        m_shaderLibrary->load("cube", SHADER_DIR "01/framebuffer.vert",
                              SHADER_DIR "01/framebuffer.frag");

        m_shaderLibrary->get("cube")->bind();
        m_shaderLibrary->get("cube")->uploadUniformInt("u_texture_0", 0);
        // m_shaderLibrary->get("cube")->uploadUniformInt("u_texture_1", 1);

        std::vector<Airwave::AWVertex> vertices_;
        std::vector<uint32_t> indices;
        Airwave::GeometryUtils::CreateCube(vertices_, indices);

        std::vector<float> vertices =
            Airwave::GeometryUtils::ConvertAWVertexToFloatArray(vertices_);

        m_vertexArray = Airwave::VertexArray::Create();
        {
            auto vertexBuffer =
                Airwave::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
            vertexBuffer->setBufferLayout({
                {Airwave::ShaderDataType::FLOAT3, "a_position"},
                {Airwave::ShaderDataType::FLOAT3, "a_normal"},
                {Airwave::ShaderDataType::FLOAT2, "a_uv"},
            });
            m_vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer =
                Airwave::IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
            m_vertexArray->setIndexBuffer(indexBuffer);
            m_vertexArray->unbind();
        }
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
            m_texture_0->bind(0);
            m_texture_1->bind(1);

            Airwave::Renderer::Submit(m_shaderLibrary->get("cube"), m_vertexArray, glm::mat4(1.0f));
            Airwave::Renderer::Submit(m_shaderLibrary->get("cube"), m_vertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)));    
        }
        Airwave::Renderer::EndScene();
        m_framebuffer->unbind();

        // 渲染全屏四边形
        m_fullScreenQuad->render(m_framebuffer);

    }

  private:
    std::shared_ptr<Airwave::Framebuffer> m_framebuffer;
    std::shared_ptr<Airwave::FullScreenQuad> m_fullScreenQuad;

    std::shared_ptr<Airwave::Texture2D> m_texture_0;
    std::shared_ptr<Airwave::Texture2D> m_texture_1;

    std::shared_ptr<Airwave::VertexArray> m_vertexArray;
    std::shared_ptr<Airwave::ShaderLibrary> m_shaderLibrary;

    std::shared_ptr<Airwave::TrackballController> m_trackballController;
    std::shared_ptr<Airwave::Camera> m_camera;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }