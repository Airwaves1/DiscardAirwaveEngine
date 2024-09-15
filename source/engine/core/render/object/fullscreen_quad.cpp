#include "fullscreen_quad.hpp"
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"

namespace Airwave
{

FullScreenQuad::FullScreenQuad()
{
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0,
    };

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

    std::string fragmentSrc = R"(
        #version 410 core
        layout(location = 0) out vec4 color;

        in vec2 v_texCoord;

        uniform sampler2D u_texture;

        void main()
        {
            color = texture(u_texture, v_texCoord);
            // color = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    m_shader = Shader::Create(vertexSrc, fragmentSrc, false);

    m_vertexArray = VertexArray::Create();
    {
        auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->setBufferLayout({
            {ShaderDataType::FLOAT2, "a_position"},
            {ShaderDataType::FLOAT2, "a_texCoord"},
        });
        m_vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices));
        m_vertexArray->setIndexBuffer(indexBuffer);
        m_vertexArray->unbind();
    }
}

void FullScreenQuad::render(const std::shared_ptr<Framebuffer> &framebuffer)
{
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();

    if (framebuffer)
    {
        if (framebuffer->getSpecification().enableMSAA)
        {
            if (m_resolveFramebuffer == nullptr)
            {
                FramebufferSpecification spec;
                spec.colorAttachmentCount   = 1;
                spec.depthAttachmentCount   = 1;
                spec.stencilAttachmentCount = 0;
                spec.enableMSAA             = false;
                spec.samples                = 1;
                m_resolveFramebuffer =
                    Framebuffer::Create(framebuffer->getWidth(), framebuffer->getHeight(), spec);
            }

            framebuffer->resolve(m_resolveFramebuffer->getFramebufferID());
            m_shader->bind();
            m_resolveFramebuffer->getColorAttachments()[0]->bind(0);
        }
        else
        {
            m_shader->bind();
            framebuffer->getColorAttachments()[0]->bind(0);
        }
    }
    else
    {
        LOG_ERROR("Framebuffer is nullptr");
        return;
    }

    m_shader->uploadUniformInt("u_texture", 0);
    RenderCommand::DrwaIndexed(m_vertexArray);
}

} // namespace Airwave
