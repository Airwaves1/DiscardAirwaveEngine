#include "graphics/vulkan/aw_vk_device.hpp"
#include "graphics/vulkan/aw_vk_context.hpp"
#include "graphics/vulkan/aw_vk_queue.hpp"

#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
namespace Airwave
{
const DeviceFeature requestedExtensions[] = {
    {VK_KHR_SWAPCHAIN_EXTENSION_NAME, true},
#ifdef AW_PLATFORM_WINDOWS
#elif AW_PLATFORM_MACOS
    {"VK_KHR_portability_subset", true},
#elif AD_ENGINE_PLATFORM_LINUX
#endif
};

AwVkDevice::AwVkDevice(AwVkContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
                       const AwVkSettings &settings)
    : m_settings(settings)
{
    // 1.验证上下文
    if (!context)
    {
        LOG_ERROR("lost vk context");
        return;
    }

    // 2.获取队列家族信息，检查队列数量，如果超出可用数量则直接返回
    QueueFamilyInfo graphicQueueInfo = context->getGraphicQueueFamily();
    QueueFamilyInfo presentQueueInfo = context->getPresentQueueFamily();
    if (graphicQueueCount > graphicQueueInfo.queueCount)
    {
        LOG_ERROR("the graphic queue has {0}, but request {1}", graphicQueueInfo.queueCount,
                  graphicQueueCount);
        return;
    }
    if (presentQueueCount > presentQueueInfo.queueCount)
    {
        LOG_ERROR("the present queue has {0}, but request {1}", presentQueueInfo.queueCount,
                  presentQueueCount);
        return;
    }

    // 3. 设置队列优先级
    std::vector<float> graphicQueuePriorities(graphicQueueCount, 0.0f);
    std::vector<float> presentQueuePriorities(presentQueueCount, 1.0f);

    // 4.
    // 检查图形和呈现队列是否属于同一个队列家族，如果是，将两个队列的数量相加，并合并它们的优先级数组
    bool bSameQueueFamily   = context->isSameGraphicAndPresentQueueFamily();
    uint32_t sameQueueCount = graphicQueueCount;
    if (bSameQueueFamily)
    {
        sameQueueCount += presentQueueCount;
        if (sameQueueCount > graphicQueueInfo.queueCount)
        {
            sameQueueCount = graphicQueueInfo.queueCount;
        }
        graphicQueuePriorities.insert(graphicQueuePriorities.end(), presentQueuePriorities.begin(),
                                      presentQueuePriorities.end());
    }

    // 5.
    // 创建队列创建信息,如果图形和呈现队列属于同一队列家族，则只创建一个队列创建信息，否则创建两个队列创建信息
    VkDeviceQueueCreateInfo queueCreateInfo[2] = {
        {.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
         .pNext            = nullptr,
         .flags            = 0,
         .queueFamilyIndex = static_cast<uint32_t>(graphicQueueInfo.queueFamilyIndex),
         .queueCount       = sameQueueCount,
         .pQueuePriorities = graphicQueuePriorities.data()}};

    if (!bSameQueueFamily)
    {
        queueCreateInfo[1] = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                              .pNext = nullptr,
                              .flags = 0,
                              .queueFamilyIndex =
                                  static_cast<uint32_t>(presentQueueInfo.queueFamilyIndex),
                              .queueCount       = presentQueueCount,
                              .pQueuePriorities = presentQueuePriorities.data()};
    }

    // 6.
    // 检查设备扩展,查询可用的设备扩展，并将其与请求的扩展进行比较。如果某些扩展不可用，函数将返回
    uint32_t availableExtensionCount = 0;
    CALL_VK(vkEnumerateDeviceExtensionProperties(context->getPhysicalDevice(), nullptr,
                                                 &availableExtensionCount, nullptr));
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    CALL_VK(vkEnumerateDeviceExtensionProperties(context->getPhysicalDevice(), nullptr,
                                                 &availableExtensionCount,
                                                 availableExtensions.data()));
    uint32_t enableExtensionCount = 0;
    const char *enableExtensions[32];
    if (!checkDeviceFeatures("Device Extensions", true, availableExtensionCount,
                             availableExtensions.data(),
                             sizeof(requestedExtensions) / sizeof(DeviceFeature),
                             requestedExtensions, &enableExtensionCount, enableExtensions))
    {
        LOG_ERROR("lost required device extensions");
        return;
    }

    // 7. 创建设备
    VkDeviceCreateInfo deviceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .queueCreateInfoCount    = static_cast<uint32_t>(bSameQueueFamily ? 1 : 2),
        .pQueueCreateInfos       = queueCreateInfo,
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = enableExtensionCount,
        .ppEnabledExtensionNames = enableExtensionCount > 0 ? enableExtensions : nullptr,
        .pEnabledFeatures        = nullptr};

    CALL_VK(vkCreateDevice(context->getPhysicalDevice(), &deviceInfo, nullptr, &m_device));
    LOG_TRACE("create device success");

    // 8.获取队列并存储
    for (int i = 0; i < graphicQueueCount; i++)
    {
        VkQueue queue;
        vkGetDeviceQueue(m_device, graphicQueueInfo.queueFamilyIndex, i, &queue);
        m_graphicQueues.emplace_back(
            std::make_shared<AwVkQueue>(graphicQueueInfo.queueFamilyIndex, i, queue, false));
    }

    for (int i = 0; i < presentQueueCount; i++)
    {
        VkQueue queue;
        vkGetDeviceQueue(m_device, presentQueueInfo.queueFamilyIndex, i, &queue);
        m_presentQueues.emplace_back(
            std::make_shared<AwVkQueue>(presentQueueInfo.queueFamilyIndex, i, queue, true));
    }

    // 9.创建管线缓存

    // 10.创建命令池
}

AwVkDevice::~AwVkDevice()
{
    vkDeviceWaitIdle(m_device);
    if (m_device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_device, nullptr);
        LOG_TRACE("destroy device success");
    }
}

} // namespace Airwave
