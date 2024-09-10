#include "graphics/vulkan/aw_vk_swapchain.hpp"
#include "graphics/vulkan/aw_vk_device.hpp"
#include "graphics/vulkan/aw_vk_context.hpp"
namespace Airwave
{

AwVkSwapchain::AwVkSwapchain(AwVkContext *context, AwVkDevice *device)
    : m_context(context), m_device(device)
{
    recreate();
}

AwVkSwapchain::~AwVkSwapchain()
{
    if (m_swapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_device->getDevice(), m_swapchain, nullptr);
    }
}

bool AwVkSwapchain::recreate()
{
    LOG_DEBUG("------------recreate swapchain------------");
    // 1. 初始化surface相关信息
    setupSurfaceCapabilities();
    LOG_DEBUG("current extent : {0} x {1}", m_surfaceInfo.capabilities.currentExtent.width,
              m_surfaceInfo.capabilities.currentExtent.height);
    LOG_DEBUG("surface format : {0}", vk_format_string(m_surfaceInfo.surfaceformats.format));
    LOG_DEBUG("present mode : {0}", vk_present_mode_string(m_surfaceInfo.presentModes));
    LOG_DEBUG("------------------------------------------");

    // 2. 设置交换链图像信息
    uint32_t imageCount = m_device->getSettings().imageCount;
    if (imageCount < m_surfaceInfo.capabilities.minImageCount &&
        m_surfaceInfo.capabilities.maxImageCount > 0)
    {
        imageCount = m_surfaceInfo.capabilities.minImageCount;
    }
    if (imageCount > m_surfaceInfo.capabilities.maxImageCount &&
        m_surfaceInfo.capabilities.maxImageCount > 0)
    {
        imageCount = m_surfaceInfo.capabilities.maxImageCount;
    }

    // 3. 确定图像共享模式和队列家族索引
    // 两者属于同一队列家族，使用独占模式
    // 不属于同一队列家族，使用并发模式,并发模式则在多个队列家族间共享图像时必不可少
    VkSharingMode imageSharingMode;
    uint32_t queueFamilyIndexCount  = 0;
    uint32_t pQueueFamilyIndices[2] = {0, 0};
    if (m_context->isSameGraphicAndPresentQueueFamily())
    {
        imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        queueFamilyIndexCount = 0;
    }
    else
    {
        imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
        queueFamilyIndexCount  = 2;
        pQueueFamilyIndices[0] = m_context->getGraphicQueueFamily().queueFamilyIndex;
        pQueueFamilyIndices[1] = m_context->getPresentQueueFamily().queueFamilyIndex;
    }

    // 4. 检查透明度

    // 5. 创建交换链信息
    VkSwapchainKHR oldSwapchain         = m_swapchain;
    VkSwapchainCreateInfoKHR createInfo = {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = nullptr,
        .flags                 = 0,
        .surface               = m_context->getSurface(),
        .minImageCount         = imageCount,
        .imageFormat           = m_surfaceInfo.surfaceformats.format,
        .imageColorSpace       = m_surfaceInfo.surfaceformats.colorSpace,
        .imageExtent           = m_surfaceInfo.capabilities.currentExtent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = imageSharingMode,
        .queueFamilyIndexCount = queueFamilyIndexCount,
        .pQueueFamilyIndices   = pQueueFamilyIndices,
        .preTransform          = m_surfaceInfo.capabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = m_surfaceInfo.presentModes,
        .clipped               = VK_FALSE,
        .oldSwapchain          = oldSwapchain,
    };

    // 检查supportedCompositeAlpha 并设置 compositeAlpha
    if (m_surfaceInfo.capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
    {
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    }
    else if (m_surfaceInfo.capabilities.supportedCompositeAlpha &
             VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
    {
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
    }
    else if (m_surfaceInfo.capabilities.supportedCompositeAlpha &
             VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
    {
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
    }
    else if (m_surfaceInfo.capabilities.supportedCompositeAlpha &
             VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
    {
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    }

    // 6. 创建新的交换链
    VkResult result =
        vkCreateSwapchainKHR(m_device->getDevice(), &createInfo, nullptr, &m_swapchain);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("{0} : failed to create swapchain", __FUNCTION__);
        return false;
    }

    // 7. 获取交换链图像
    uint32_t swapchainImageCount = 0;
    result =
        vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapchain, &swapchainImageCount, nullptr);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("{0} : failed to get swapchain images", __FUNCTION__);
        return false;
    }
    m_images.resize(swapchainImageCount);
    result = vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapchain, &swapchainImageCount,
                                     m_images.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("{0} : failed to get swapchain images", __FUNCTION__);
        return false;
    }

    return result == VK_SUCCESS;
}

void AwVkSwapchain::setupSurfaceCapabilities()
{
    // 1. 获取surface能力,VkSurfaceCapabilitiesKHR
    // 包含了表面的一些关键属性，例如表面支持的最小和最大图像数量、图像的宽高范围、当前变换等。
    CALL_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        m_context->getPhysicalDevice(), m_context->getSurface(), &m_surfaceInfo.capabilities));

    // 2. 获取surface格式,定义了图像的像素格式和色彩空间
    uint32_t formatCount = 0;
    CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_context->getPhysicalDevice(),
                                                 m_context->getSurface(), &formatCount, nullptr));
    if (formatCount < 0)
    {
        LOG_ERROR("{0} : number of surface formats is 0", __FUNCTION__);
        return;
    }
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(m_context->getPhysicalDevice(),
                                                 m_context->getSurface(), &formatCount,
                                                 surfaceFormats.data()));

    // 3. 选择合适的 Surface 格式
    // 选择一个与应用程序期望的格式和色彩空间匹配的表面格式。
    // 如果没有找到完全匹配的格式，则选择第一个可用的格式作为默认值
    int32_t foundFormatIndex = -1;
    for (uint32_t i = 0; i < formatCount; i++)
    {
        if (surfaceFormats[i].format == m_device->getSettings().surfaceFormat)
        {
            foundFormatIndex = i;
            break;
        }
    }
    if (foundFormatIndex < 0)
    {
        foundFormatIndex = 0;
    }
    m_surfaceInfo.surfaceformats = surfaceFormats[foundFormatIndex];

    // 4. 获取呈现模式
    // 指定了图像的显示方式（如 FIFO、Mailbox 等）。不同的呈现模式可以影响图像的延迟和同步效果
    uint32_t presentModeCount = 0;
    CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(
        m_context->getPhysicalDevice(), m_context->getSurface(), &presentModeCount, nullptr));
    if (presentModeCount == 0)
    {
        LOG_ERROR("{0} : number of present modes is 0", __FUNCTION__);
        return;
    }
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(m_context->getPhysicalDevice(),
                                                      m_context->getSurface(), &presentModeCount,
                                                      presentModes.data()));

    // 5. 选择合适的呈现模式
    int32_t foundPresentModeIndex = -1;
    for (uint32_t i = 0; i < presentModeCount; i++)
    {
        if (presentModes[i] == m_device->getSettings().presentMode)
        {
            foundPresentModeIndex = i;
            break;
        }
    }
    if (foundPresentModeIndex < 0)
    {
        foundPresentModeIndex = 0;
    }
    m_surfaceInfo.presentModes = presentModes[foundPresentModeIndex];
}

} // namespace Airwave
