#include "airwave_engine.hpp"

int main()
{
    Airwave::Log::Init();

    auto window = Airwave::AwWindow::Create(800, 600, "AirwaveEngine");

    auto shaderLibrary = std::make_shared<Airwave::ShaderLibrary>();
    shaderLibrary->load("triangle", SHADER_DIR "00/triangle.vert", SHADER_DIR "00/triangle.frag");

    float vertices[3 * 6] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.3f, 0.2f, 0.5f, -0.5f, 0.0f,
                             0.2f,  0.3f,  0.8f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f,  0.2f};

    uint32_t indices[3] = {0, 1, 2};

    auto vertexArray = Airwave::VertexArray::Create();
    {
        auto vertexBuffer = Airwave::VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->setBufferLayout({{Airwave::ShaderDataType::FLOAT3, "a_position"},
                                       {Airwave::ShaderDataType::FLOAT3, "a_color"}});
        vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer =
            Airwave::IndexBuffer::Create(indices, sizeof(indices) );
        vertexArray->setIndexBuffer(indexBuffer);
    }



    Airwave::RenderCommand::SetClearColor({0.6f, 0.4f, 0.7f, 1.0f});

    auto shader = shaderLibrary->get("triangle");

    while (!window->shouldClose())
    {
        Airwave::RenderCommand::Clear();

        shader->bind();
        Airwave::RenderCommand::DrwaIndexed(vertexArray);

        window->pollEvents();
        window->swapBuffers();
    }

    return 0;
}