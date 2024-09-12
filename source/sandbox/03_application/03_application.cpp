#include "airwave_engine.hpp"
#include <glm/glm.hpp>

// a_position, a_normal
std::vector<float> vertices = {
    // Front face
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // 1
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 2
    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 3

    // Back face
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 4
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // 5
    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // 6
    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // 7

    // Right face
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
    1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 9
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 10
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 11

    // Left face
    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
    -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 13
    -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 14
    -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 15

    // Top face
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 16
    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 17
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // 18
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 19

    // Bottom face
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 20
    1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // 21
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // 22
    -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f   // 23
};

std::vector<uint32_t> indices = {
    0,  1,  2,  2,  3,  0,  // front
    4,  5,  6,  6,  7,  4,  // back
    8,  9,  10, 10, 11, 8,  // right
    12, 13, 14, 14, 15, 12, // left
    16, 17, 18, 18, 19, 16, // top
    20, 21, 22, 22, 23, 20  // bottom
};

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