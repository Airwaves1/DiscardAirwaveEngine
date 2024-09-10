#include "graphics/vulkan/aw_vk_device.hpp"
#include "graphics/vulkan/aw_vk_context.hpp"
namespace Airwave
{

AwVkDevice::AwVkDevice(AwVkContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
                       const AwVkSettings &settings)
{
    if (!context)
    {
        LOG_ERROR("lost vk context");
        return;
    }

    QueueFamilyInfo graphicQueueInfo = context->getGraphicQueueFamily();
    QueueFamilyInfo presentQueueInfo = context->getPresentQueueFamily();    
    if(graphicQueueCount > graphicQueueInfo.queueCount)
    {
        LOG_ERROR("the graphic queue has {0}, but request {1}", graphicQueueInfo.queueCount, graphicQueueCount);
        return;
    }
    if(presentQueueCount > presentQueueInfo.queueCount)
    {
        LOG_ERROR("the present queue has {0}, but request {1}", presentQueueInfo.queueCount, presentQueueCount);
        return;
    }

    std::vector<float> graphicQueuePriorities(graphicQueueCount, 0.0f);
    std::vector<float> presentQueuePriorities(presentQueueCount, 1.0f);


    VkDeviceCreateInfo deviceCreatInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 2,
        .pQueueCreateInfos = nullptr,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = nullptr,
    };

    CALL_VK(vkCreateDevice(context->getPhysicalDevice(), &deviceCreatInfo, nullptr, &m_device));

}

AwVkDevice::~AwVkDevice() {}

} // namespace Airwave
