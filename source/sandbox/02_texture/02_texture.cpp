#include "window/aw_window.hpp"
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"
#include "render/renderer/renderer_api.hpp"
#include "render/shader/shader.hpp"
#include "render/shader/shader_library.hpp"
#include "render/buffer/vertex_buffer.hpp"
#include "render/buffer/vertex_array.hpp"
#include "render/texture/texture.hpp"
#include "render/texture/texture_2d.hpp"
#include "utils/common.hpp"
#include "utils/log.hpp"
#include "utils/file_utils.hpp"
#include <glm/glm.hpp>
int main()
{
    Airwave::Log::Init();

    auto window = Airwave::AwWindow::Create(1200, 900, "02_texture");

    auto texture_0 = Airwave::Texture2D::Create(TEXTURE_DIR "container2.png");
    auto texture_1 = Airwave::Texture2D::Create(TEXTURE_DIR "R-C.jpeg");

    auto shaderLibrary = std::make_shared<Airwave::ShaderLibrary>();
    shaderLibrary->load("cube", SHADER_DIR "00/cube_texture.vert",
                        SHADER_DIR "00/cube_texture.frag");

    shaderLibrary->get("cube")->bind();
    shaderLibrary->get("cube")->uploadUniformInt("u_texture_0", 0);
    shaderLibrary->get("cube")->uploadUniformInt("u_texture_1", 1);

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

    auto vertexArray = Airwave::VertexArray::Create();
    {
        auto vertexBuffer =
            Airwave::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
        vertexBuffer->setBufferLayout({
            {Airwave::ShaderDataType::FLOAT3, "a_position"},
            {Airwave::ShaderDataType::FLOAT3, "a_normal"},
            {Airwave::ShaderDataType::FLOAT2, "a_uv"},
        });
        vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer =
            Airwave::IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->unbind();
    }

    uint32_t rotation = 0;

    while (!window->shouldClose())
    {
        if (rotation == 999999999) rotation = 0;
        rotation++;

        Airwave::RenderCommand::SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
        Airwave::RenderCommand::Clear();
        Airwave::RenderCommand::Enable(Airwave::RenderState::DepthTest);

        auto shader = shaderLibrary->get("cube");
        shader->bind();
        shader->uploadUniformMat4("u_view", glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f),
                                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                                        glm::vec3(0.0f, 1.0f, 0.0f)));
        shader->uploadUniformMat4(
            "u_projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
        shader->uploadUniformMat4("u_model",
                                  glm::rotate(glm::mat4(1.0f), glm::radians((float)rotation),
                                              glm::vec3(0.5f, 1.0f, 0.0f)));

        texture_0->bind(0);
        texture_1->bind(1);
        vertexArray->bind();
        Airwave::RenderCommand::DrwaIndexed(vertexArray);

        window->pollEvents();
        window->swapBuffers();
    }

    return 0;
}
