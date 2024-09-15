// #include "fullscreen_quad.hpp"
// #include "render/renderer/renderer.hpp"
// #include "render/renderer/render_command.hpp"

// namespace Airwave
// {

// FullScreenQuad::FullScreenQuad()
// {
//     float vertices[] = {
//         -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
//     };

//     uint32_t indices[] = {
//         0, 1, 2, 2, 3, 0,
//     };

//     std::string vertexSrc = R"(
//         #version 410 core
//         layout(location = 0) in vec2 a_position;
//         layout(location = 1) in vec2 a_texCoord;

//         out vec2 v_texCoord;

//         void main()
//         {
//             gl_Position = vec4(a_position, 0.0, 1.0);
//             v_texCoord = a_texCoord;
//         }
//     )";

//     std::string fragmentSrc = R"(
//         #version 410 core
//         layout(location = 0) out vec4 color;

//         in vec2 v_texCoord;

//         uniform sampler2D u_texture;

//         void main()
//         {
//             color = texture(u_texture, v_texCoord);
//         }
//     )";

//     // m_shader = Shader::Create(vertexSrc, fragmentSrc, false);

//     m_vertexArray = VertexArray::Create();
//     {
//         auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
//         vertexBuffer->setBufferLayout({
//             {ShaderDataType::FLOAT2, "a_position"},
//             {ShaderDataType::FLOAT2, "a_texCoord"},
//         });
//         m_vertexArray->addVertexBuffer(vertexBuffer);

//         auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
//         m_vertexArray->setIndexBuffer(indexBuffer);
//         m_vertexArray->unbind();
//     }
// }

// void FullScreenQuad::render(const std::shared_ptr<Texture2D> &texture)
// {
//     Airwave::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
//     Airwave::RenderCommand::Clear();

//     m_shader->bind();
//     texture->bind(0);
//     m_shader->uploadUniformInt("u_texture", 0);
//     Airwave::Renderer::Submit(m_shader, m_vertexArray, glm::mat4(1.0f));
// }

// } // namespace Airwave
