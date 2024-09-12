#pragma once

#include "graphics/graphic_contex.hpp"
#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{

struct QueueFamilyInfo
{
    int32_t queueFamilyIndex = -1;
    uint32_t queueCount      = 0;
};

class AwVkContext : public GraphicContext
{
  public:
    AwVkContext() = delete;
    AwVkContext(AwWindow *window);
    ~AwVkContext() override;

    void swapBuffers() override {}
    VkInstance getInstance() const { return m_instance; }
    VkSurfaceKHR getSurface() const { return m_surface; }
    VkPhysicalDevice getPhysicalDevice() const { return m_phyDevice; }
    const QueueFamilyInfo &getGraphicQueueFamily() const { return m_graphicQueueFamily; }
    const QueueFamilyInfo &getPresentQueueFamily() const { return m_presentQueueFamily; }
    bool isSameGraphicAndPresentQueueFamily() const
    {
        return m_graphicQueueFamily.queueFamilyIndex == m_presentQueueFamily.queueFamilyIndex;
    }

  private:
    void createInstance();
    void createSurface(AwWindow *window);
    void pickPhysicalDevice();

    void printPhysicalDeviceProperties(VkPhysicalDeviceProperties &properties);
    uint32_t getPhyDeviceScore(VkPhysicalDeviceProperties &props);

  private:
    bool m_needValidationLayer = true;
    VkInstance m_instance      = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface     = VK_NULL_HANDLE;

    VkPhysicalDevice m_phyDevice = VK_NULL_HANDLE;
    QueueFamilyInfo m_graphicQueueFamily;
    QueueFamilyInfo m_presentQueueFamily;
    VkPhysicalDeviceMemoryProperties m_phyDeviceMemoryProperties;
};

} // namespace Airwave
