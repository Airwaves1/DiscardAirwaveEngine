#include "fullscreen_quad.hpp"
#include "render/renderer/renderer.hpp"
#include "render/renderer/render_command.hpp"
#include "render/shader/shader_library.hpp"

namespace Airwave
{

FullScreenQuad::FullScreenQuad()
{
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
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

        uniform sampler2D u_depthTexture;
        uniform sampler2D u_texture;

        void main()
        {
            color = texture(u_texture, v_texCoord);
            // color = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    m_shader = SHADER_LIB.load("fullscreen_quad", vertexSrc, fragmentSrc, false);

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

void FullScreenQuad::resolve(const std::shared_ptr<Framebuffer> &framebuffer)
{
    if (framebuffer->getSpecification().enableMSAA)
    {
        if (!m_resolveFramebuffer)
        {
            FramebufferSpecification spec;
            spec.colorAttachmentCount   = framebuffer->getSpecification().colorAttachmentCount;
            spec.depthAttachmentCount   = 1;
            spec.stencilAttachmentCount = 0;
            spec.enableMSAA             = false;
            spec.samples                = 1;
            m_resolveFramebuffer        = Framebuffer::Create(framebuffer->getWidth(), framebuffer->getHeight(), spec);
        }

        if (m_resolveFramebuffer->getWidth() != framebuffer->getWidth() ||
            m_resolveFramebuffer->getHeight() != framebuffer->getHeight())
        {
            m_resolveFramebuffer->resize(framebuffer->getWidth(), framebuffer->getHeight());
        }
        // 解析多重采样的附件
        framebuffer->resolve(m_resolveFramebuffer->getFramebufferID());
    }
    else
    {

        m_resolveFramebuffer = framebuffer;
    }
}

void FullScreenQuad::renderToDefaultFramebuffer()
{
    if (!m_resolveFramebuffer)
    {
        LOG_ERROR("FullScreenQuad Framebuffer is nullptr");
        return;
    }

    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();
    RenderCommand::Disable(RenderState::DepthTest);
    m_shader->bind();

    auto colorAttachment = m_resolveFramebuffer->getColorAttachments()[0];
    if (colorAttachment)
    {
        colorAttachment->bind(0);
        m_shader->uploadUniformInt("u_texture", 0);
    }
    else
    {
        LOG_ERROR("Color attachment is nullptr");
    }

    auto depthAttachment = m_resolveFramebuffer->getDepthAttachment();
    if (depthAttachment)
    {
        depthAttachment->bind(1);
        m_shader->uploadUniformInt("u_depthTexture", 1);
    }

    RenderCommand::DrawIndexed(m_vertexArray);
}

void FullScreenQuad::render(const std::shared_ptr<Texture2D> &texture, const std::shared_ptr<Shader> &shader)
{
    if (!m_resolveFramebuffer)
    {
        LOG_ERROR("FullScreenQuad Framebuffer is nullptr");
        return;
    }

    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear();
    RenderCommand::Disable(RenderState::DepthTest);

    m_resolveFramebuffer->bind();

    shader->bind();

    if (texture)
    {
        texture->bind(0);
        shader->uploadUniformInt("u_texture", 0);
    }
    else
    {
        m_resolveFramebuffer->getColorAttachments()[0]->bind(0);
    }

    auto depthAttachment = m_resolveFramebuffer->getDepthAttachment();
    if (depthAttachment)
    {
        depthAttachment->bind(1);
        shader->uploadUniformInt("u_depthTexture", 1);
    }

    RenderCommand::DrawIndexed(m_vertexArray);

    m_resolveFramebuffer->unbind();
}

} // namespace Airwave
