#pragma once

#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{

class AwVkDevice;
class AwVkContext;

struct SurfaceInfo
{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR surfaceformats;
    VkPresentModeKHR presentModes;
};

class AwVkSwapchain
{
  public:
    AwVkSwapchain(AwVkContext *context, AwVkDevice *device);
    ~AwVkSwapchain();

    bool recreate();

    VkResult acquireNextImage(uint64_t timeout, VkSemaphore semaphore, VkFence fence,
                              uint32_t *pImageIndex);
    VkResult present(uint32_t imageIndex, VkSemaphore waitSemaphore);

    const std::vector<VkImage> &getImages() const { return m_images; }
    uint32_t getWidth() const { return m_surfaceInfo.capabilities.currentExtent.width; }
    uint32_t getHeight() const { return m_surfaceInfo.capabilities.currentExtent.height; }
    int32_t getCurrentImageIndex() const { return m_currentImageIndex; }

    const VkSwapchainKHR &getSwapchain() const { return m_swapchain; }

  private:
    void setupSurfaceCapabilities();

  private:
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    AwVkContext *m_context     = nullptr;
    AwVkDevice *m_device       = nullptr;

    std::vector<VkImage> m_images;

    int32_t m_currentImageIndex = -1;

    SurfaceInfo m_surfaceInfo;
};
} // namespace Airwave