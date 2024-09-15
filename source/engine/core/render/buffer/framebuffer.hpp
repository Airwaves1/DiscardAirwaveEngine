#pragma once
#include "utils/common.hpp"
#include "render/texture/texture.hpp"
#include "render/texture/texture_2d.hpp"
#include <unordered_map>
#include <glm/glm.hpp>

namespace Airwave
{

struct FramebufferSpecification
{
    uint32_t colorAttachmentCount   = 1;
    uint32_t depthAttachmentCount   = 1;
    uint32_t stencilAttachmentCount = 0;
    bool enableMSAA                 = false;
    uint16_t samples                = 1;

    FramebufferSpecification(uint32_t colorAttachmentCount = 1, uint32_t depthAttachmentCount = 1,
                             uint32_t stencilAttachmentCount = 0, bool enableMSAA = false)
        : colorAttachmentCount(colorAttachmentCount), depthAttachmentCount(depthAttachmentCount),
          stencilAttachmentCount(stencilAttachmentCount), enableMSAA(enableMSAA)
    {
    }
};

class Framebuffer
{

  public:
    static std::shared_ptr<Framebuffer> Create(uint32_t width, uint32_t height,
                                               const FramebufferSpecification &spec);
    virtual ~Framebuffer() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getFramebufferID() const = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    // 获取附件
    virtual std::vector<std::shared_ptr<Texture2D>> getColorAttachments() const = 0;
    virtual std::shared_ptr<Texture2D> getDepthAttachment() const               = 0;
    virtual std::shared_ptr<Texture2D> getStencilAttachment() const             = 0;

    // 设置多重采样
    virtual void setMSAA(bool enable) = 0;

    // 将多重采样的纹理转换为普通附件
    virtual void resolve(const uint32_t resolveFramebufferID) const = 0;

    // 读取像素获取单点颜色
    virtual glm::vec4 readPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) const = 0;

    virtual uint32_t getWidth() const { return m_width; }
    virtual uint32_t getHeight() const { return m_height; }

    virtual const FramebufferSpecification &getSpecification() const { return m_spec; }

  protected:
    uint32_t m_width;
    uint32_t m_height;
    FramebufferSpecification m_spec;
};

} // namespace Airwave