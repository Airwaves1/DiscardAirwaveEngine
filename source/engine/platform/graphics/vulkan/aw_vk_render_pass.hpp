#pragma once

#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{

class AwVkDevice;
class AwVkFramebuffer;

struct Attachment
{
    VkFormat format                    = VK_FORMAT_UNDEFINED;
    VkAttachmentLoadOp loadOp          = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VkAttachmentLoadOp stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VkImageLayout initialLayout        = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout finalLayout          = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits samples      = VK_SAMPLE_COUNT_1_BIT;
    VkImageUsageFlags usage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
};

struct RenderSubPass
{
    std::vector<uint32_t> inputAttachments;
    std::vector<uint32_t> colorAttachments;
    std::vector<uint32_t> dpethStencilAttchments;
    VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
};

class AwVkRenderPass
{
  public:
    AwVkRenderPass(AwVkDevice *device, const std::vector<Attachment> &attachments = {},
                   const std::vector<RenderSubPass> &subPasses = {});
    ~AwVkRenderPass();

    VkRenderPass getHandle() const { return m_renderPass; }

  private:
    VkRenderPass m_renderPass = VK_NULL_HANDLE;

    AwVkDevice *m_device;

    std::vector<Attachment> m_attachments;
    std::vector<RenderSubPass> m_subPasses;
};
} // namespace Airwave
