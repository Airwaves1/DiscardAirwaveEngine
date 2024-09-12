#include "graphics/vulkan/aw_vk_render_pass.hpp"
#include "graphics/vulkan/aw_vk_device.hpp"
namespace Airwave
{
AwVkRenderPass::AwVkRenderPass(AwVkDevice *device, const std::vector<Attachment> &attachments,
                               const std::vector<RenderSubPass> &subPasses)
    : m_device(device), m_attachments(attachments), m_subPasses(subPasses)
{
    // 1. 默认子通道和附件的处理
    // 如果传入的subPass为空，创建一个默认的附件和一个默认的子通道
    if (m_subPasses.empty())
    {
        m_attachments = {{
            .format       = m_device->getSettings().surfaceFormat, 
            .loadOp       = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp      = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
            .samples      = VK_SAMPLE_COUNT_1_BIT,           
            .usage        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        }};
        m_subPasses   = {{
              .colorAttachments = {0},
              .sampleCount      = VK_SAMPLE_COUNT_1_BIT,
        }};
    }

    // 2. 创建子通道描述
    std::vector<VkSubpassDescription> subpassDescription(m_subPasses.size());
    std::vector<VkAttachmentReference> resolveAttachmentReferences(m_subPasses.size());
    std::vector<std::vector<VkAttachmentReference>> inputAttachmentReferences(m_subPasses.size());
    std::vector<std::vector<VkAttachmentReference>> colorAttachmentReferences(m_subPasses.size());
    std::vector<std::vector<VkAttachmentReference>> depthStencilAttachmentReferences(
        m_subPasses.size());
    // 2.1 遍历子通道
    for (int i = 0; i < m_subPasses.size(); i++)
    {
        RenderSubPass subpass           = m_subPasses[i];
        auto &subpassDesc               = subpassDescription[i];
        auto &inputAttachmentRef        = inputAttachmentReferences[i];
        auto &colorAttachmentRef        = colorAttachmentReferences[i];
        auto &depthStencilAttachmentRef = depthStencilAttachmentReferences[i];

        for (auto index : subpass.inputAttachments)
        {
            inputAttachmentRef.push_back(
                {.attachment = index, .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL});
        }

        for (auto index : subpass.colorAttachments)
        {
            colorAttachmentRef.push_back(
                {.attachment = index, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
            m_attachments[index].samples = subpass.sampleCount;
            if (subpass.sampleCount != VK_SAMPLE_COUNT_1_BIT)
            {
                m_attachments[index].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
        }

        for (auto index : subpass.dpethStencilAttchments)
        {
            depthStencilAttachmentRef.push_back(
                {.attachment = index, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
            m_attachments[index].samples = subpass.sampleCount;
        }
        // 如果子渲染流程启用多重采样,则需要创建一个附加的解析附件,并且将它作为子通道描述的一部分
        if (subpass.sampleCount > VK_SAMPLE_COUNT_1_BIT)
        {
            m_attachments.push_back({
                .format         = m_device->getSettings().surfaceFormat,
                .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .usage          = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            });

            resolveAttachmentReferences[i] = {
                .attachment = static_cast<uint32_t>(m_attachments.size() - 1),
                .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };
        }
        subpassDescription[i] = {
            .flags                   = 0,
            .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount    = static_cast<uint32_t>(inputAttachmentRef.size()),
            .pInputAttachments       = inputAttachmentRef.data(),
            .colorAttachmentCount    = static_cast<uint32_t>(colorAttachmentRef.size()),
            .pColorAttachments       = colorAttachmentRef.data(),
            .pResolveAttachments     = subpass.sampleCount > VK_SAMPLE_COUNT_1_BIT
                                           ? resolveAttachmentReferences.data()
                                           : nullptr,
            .pDepthStencilAttachment = depthStencilAttachmentRef.data(),
            .preserveAttachmentCount = 0,
            .pPreserveAttachments    = nullptr,
        };
    }

    // 3. 处理子通道依赖
    // 如果有多个子通道，代码会创建子通道之间的依赖关系，确保前一个子通道的输出能够正确传递到下一个子通道中使用
    // 这里设置了依赖的阶段掩码（srcStageMask和dstStageMask）和访问掩码（srcAccessMask和dstAccessMask），
    // 表明颜色附件的输出会在片段着色器阶段使用
    std::vector<VkSubpassDependency> dependencies(m_subPasses.size() - 1);
    if (m_subPasses.size() > 1)
    {
        for (uint32_t i = 0; i < m_subPasses.size() - 1; i++)
        {
            dependencies[i] = {
                .srcSubpass      = i,
                .dstSubpass      = i + 1,
                .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
            };
        }
    }

    // 4. 创建渲染通道
    std::vector<VkAttachmentDescription> attachmentDescriptions(m_attachments.size());
    for (const auto &attachment : m_attachments)
    {
        attachmentDescriptions.push_back({.flags          = 0,
                                          .format         = attachment.format,
                                          .samples        = attachment.samples,
                                          .loadOp         = attachment.loadOp,
                                          .storeOp        = attachment.storeOp,
                                          .stencilLoadOp  = attachment.stencilLoadOp,
                                          .stencilStoreOp = attachment.stencilStoreOp,
                                          .initialLayout  = attachment.initialLayout,
                                          .finalLayout    = attachment.finalLayout});
    }

    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size()),
        .pAttachments    = attachmentDescriptions.data(),
        .subpassCount    = static_cast<uint32_t>(m_subPasses.size()),
        .pSubpasses      = subpassDescription.data(),
        .dependencyCount = static_cast<uint32_t>(dependencies.size()),
        .pDependencies   = dependencies.data()};

    CALL_VK(
        vkCreateRenderPass(m_device->getDevice(), &renderPassCreateInfo, nullptr, &m_renderPass));
    LOG_TRACE("RenderPass {0} : create {1} attachments, {2} subpasses, {3} dependencies",
              (void *)m_renderPass, attachmentDescriptions.size(), subpassDescription.size(),
              dependencies.size());
}
AwVkRenderPass::~AwVkRenderPass()
{
    if (m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(m_device->getDevice(), m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
}
} // namespace Airwave