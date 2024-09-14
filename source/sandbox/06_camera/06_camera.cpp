#include "airwave_engine.hpp"
#include <glm/glm.hpp>

class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {
        m_camera = std::make_shared<Airwave::PerspectiveCamera>(
            45.0f, static_cast<float>(1200) / 900, 0.1f, 100.0f);
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
        m_trackballController = std::make_shared<Airwave::TrackballController>(m_camera);

        m_texture_0 = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
        m_texture_1 = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg");

        m_shaderLibrary = std::make_shared<Airwave::ShaderLibrary>();
        m_shaderLibrary->load("cube", SHADER_DIR "00/cube_texture.vert",
                              SHADER_DIR "00/cube_texture.frag");

        m_shaderLibrary->get("cube")->bind();
        m_shaderLibrary->get("cube")->uploadUniformInt("u_texture_0", 0);
        m_shaderLibrary->get("cube")->uploadUniformInt("u_texture_1", 1);

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
        m_camera->updateViewMatrix();

        static int rotation = 0;
        if (rotation == 999999999) rotation = 0;
        rotation++;
        Airwave::RenderCommand::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
        Airwave::RenderCommand::Clear();
        Airwave::RenderCommand::Enable(Airwave::RenderState::DepthTest);

        auto shader = m_shaderLibrary->get("cube");
        shader->bind();

        shader->uploadUniformMat4("u_view", m_camera->getViewMatrix());
        shader->uploadUniformMat4("u_projection", m_camera->getProjectionMatrix());
        shader->uploadUniformMat4("u_model", glm::mat4(1.0f));


        m_texture_0->bind(0);
        m_texture_1->bind(1);
        m_vertexArray->bind();
        Airwave::RenderCommand::DrwaIndexed(m_vertexArray);
    }

  private:
    std::shared_ptr<Airwave::Texture2D> m_texture_0;
    std::shared_ptr<Airwave::Texture2D> m_texture_1;

    std::shared_ptr<Airwave::VertexArray> m_vertexArray;
    std::shared_ptr<Airwave::ShaderLibrary> m_shaderLibrary;

    std::shared_ptr<Airwave::TrackballController> m_trackballController;
    std::shared_ptr<Airwave::Camera> m_camera;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }