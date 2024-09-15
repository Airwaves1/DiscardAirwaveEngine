#pragma once
#include "render/buffer/framebuffer.hpp"

namespace Airwave
{

class OpenGLFramebuffer : public Framebuffer
{
  public:
    OpenGLFramebuffer(uint32_t width, uint32_t height, const FramebufferSpecification &spec);
    virtual ~OpenGLFramebuffer() = default;

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t getFramebufferID() const override { return m_framebufferID; }

    virtual void resize(uint32_t width, uint32_t height) override;

    virtual std::vector<std::shared_ptr<Texture2D>> getColorAttachments() const override
    {
        return std::vector<std::shared_ptr<Texture2D>>(m_colorAttachments.begin(), m_colorAttachments.end());
    }
    virtual std::shared_ptr<Texture2D> getDepthAttachment() const override
    {
        return m_depthAttachment;
    }
    virtual std::shared_ptr<Texture2D> getStencilAttachment() const override
    {
        return m_stencilAttachment;
    }

    virtual void setMSAA(bool enable) override;

    virtual void resolve(const uint32_t resolveFramebufferID) const override;

    virtual glm::vec4 readPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const override;

  private:
    void invalidate(); // 用于初始化或者重新创建Framebuffer

    uint32_t m_framebufferID = 0;

    std::vector<std::shared_ptr<Texture2D>> m_colorAttachments;
    std::shared_ptr<Texture2D> m_depthAttachment;
    std::shared_ptr<Texture2D> m_stencilAttachment;
    uint32_t m_renderbufferid = 0;
};

} // namespace Airwave