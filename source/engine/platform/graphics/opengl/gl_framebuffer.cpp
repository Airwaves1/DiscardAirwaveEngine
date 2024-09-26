#include "graphics/opengl/gl_framebuffer.hpp"
#include "gl_framebuffer.hpp"
#include "gl_texture.hpp"
#include <glad/glad.h>
#include <utils/log.hpp>

namespace Airwave
{
OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, const FramebufferSpecification &spec)
{
    m_width  = width;
    m_height = height;
    m_spec   = spec;

    invalidate();
}

void OpenGLFramebuffer::invalidate()
{
    if (m_framebufferID)
    {
        // 删除之前的附件
        if (m_depthAttachment)
        {
            GLuint depthAttachmentID = m_depthAttachment->getRendererID();
            glDeleteTextures(1, &depthAttachmentID);
        }
        if (m_stencilAttachment)
        {
            GLuint stencilAttachmentID = m_stencilAttachment->getRendererID();
            glDeleteTextures(1, &stencilAttachmentID);
        }
        if (m_renderbufferid)
        {
            glDeleteRenderbuffers(1, &m_renderbufferid);
        }
        // 删除帧缓冲
        glDeleteFramebuffers(1, &m_framebufferID);
    }

    // 生成新的帧缓冲
    glGenFramebuffers(1, &m_framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    // 创建并附加颜色附件
    m_colorAttachments.clear();
    m_colorAttachments.reserve(m_spec.colorAttachmentCount);
    for (uint32_t i = 0; i < m_spec.colorAttachmentCount; i++)
    {
        TextureSpecification spec;
        spec.enableMSAA     = m_spec.enableMSAA;
        spec.samples        = m_spec.enableMSAA ? m_spec.samples : 1;
        // spec.internalFormat = TextureInternalFormat::RGBA8;
        spec.internalFormat = TextureInternalFormat::RGBA16F;
        spec.format          = TextureFormat::RGBA;
        auto colorAttachment = Texture2D::Create(m_width, m_height, spec);
        if (m_spec.enableMSAA)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE,
                                   colorAttachment->getRendererID(), 0);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                   colorAttachment->getRendererID(), 0);
        }
        m_colorAttachments.push_back(colorAttachment);
    }

    // 创建并附加深度和/或模板附件
    if (m_spec.depthAttachmentCount > 0 || m_spec.stencilAttachmentCount > 0)
    {
        if (m_spec.depthAttachmentCount > 0 && m_spec.stencilAttachmentCount > 0)
        {
            // 使用renderbuffer作为深度模板附件
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbufferid);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferid);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples, GL_DEPTH24_STENCIL8, m_width,
                                                 m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                          m_renderbufferid);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24STENCIL8;
                spec.format         = TextureFormat::DEPTH;

                if (m_depthAttachment)
                {
                    m_depthAttachment.reset();
                }
                m_depthAttachment = Texture2D::Create(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                       m_depthAttachment->getRendererID(), 0);
            }
        }
        else if (m_spec.depthAttachmentCount > 0)
        {
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbufferid);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferid);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples, GL_DEPTH_COMPONENT24, m_width,
                                                 m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferid);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24;
                spec.format         = TextureFormat::DEPTH;

                if (m_depthAttachment)
                {
                    m_depthAttachment.reset();
                }
                m_depthAttachment = Texture2D::Create(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                       m_depthAttachment->getRendererID(), 0);
            }
        }
        else if (m_spec.stencilAttachmentCount > 0)
        {
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbufferid);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferid);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples, GL_STENCIL_INDEX8, m_width, m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferid);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24STENCIL8;
                spec.format         = TextureFormat::DEPTH;

                if (m_stencilAttachment)
                {
                    m_stencilAttachment.reset();
                }
                m_stencilAttachment = Texture2D::Create(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                       m_stencilAttachment->getRendererID(), 0);
            }
        }
    }

    // 检查帧缓冲状态
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        AW_ASSERT(false, "Framebuffer is not complete!");
    }

    // 解绑帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID); }

void OpenGLFramebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
{
    m_width  = width;
    m_height = height;
    invalidate(); // 重新创建帧缓冲
}

void OpenGLFramebuffer::setMSAA(bool enable)
{
    m_spec.enableMSAA = enable;
    invalidate(); // 重新创建帧缓冲
}

void OpenGLFramebuffer::resolve(const uint32_t resolveFramebufferID) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolveFramebufferID);
    // 解析颜色附件和深度附件时，要注意分开解析,因为深度附件似乎必须只能用GL_NEAREST
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec4 OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

    glm::vec4 pixel;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pixel);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return pixel;
}

} // namespace Airwave
