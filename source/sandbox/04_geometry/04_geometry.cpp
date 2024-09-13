#include "airwave_engine.hpp"
#include <glm/glm.hpp>


class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    void onInit() override
    {
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
        // Airwave::GeometryUtils::CreateCube(vertices_, indices);
        // Airwave::GeometryUtils::CreatePlane(vertices_, indices);
        // Airwave::GeometryUtils::CreateSphere(vertices_, indices, 1.0f, 32, 32);
        // Airwave::GeometryUtils::CreateCylinder(vertices_, indices, 1.0f, 1.0f, 1.0f, 32, 1);
        Airwave::GeometryUtils::CreateTorus(vertices_, indices, 1.0f, 0.4f, 32, 32);
        // Airwave::GeometryUtils::CreateTorusKnot(vertices_, indices, 1.0f, 0.4f, 64, 8, 2, 3);
        // Airwave::GeometryUtils::CreateCone(vertices_, indices, 1.0f, 1.0f, 32, 1);

        std::vector<float> vertices = Airwave::GeometryUtils::ConvertAWVertexToFloatArray(vertices_);

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

        static int rotation = 0;
        if (rotation == 999999999) rotation = 0;
        rotation++;
        Airwave::RenderCommand::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
        Airwave::RenderCommand::Clear();
        Airwave::RenderCommand::Enable(Airwave::RenderState::DepthTest);

        auto shader = m_shaderLibrary->get("cube");
        shader->bind();
        shader->uploadUniformMat4("u_view", glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f),
                                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                                        glm::vec3(0.0f, 1.0f, 0.0f)));
        shader->uploadUniformMat4(
            "u_projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
        shader->uploadUniformMat4("u_model",
                                  glm::rotate(glm::mat4(1.0f), glm::radians((float)rotation),
                                              glm::vec3(0.5f, 1.0f, 0.0f)));

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
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }