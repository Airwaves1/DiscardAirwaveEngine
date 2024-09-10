#pragma once

#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{

struct AwVkSettings
{
    VkFormat surfaceFormat       = VK_FORMAT_B8G8R8A8_UNORM; // 表示交换链图像的颜色格式
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // 表示交换链的呈现模式
    uint32_t imageCount          = 3;                             // 表示交换链图像的数量
};

class AwVkQueue;
class AwVkContext;
class AwVkDevice
{
  public:
    AwVkDevice(AwVkContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
               const AwVkSettings &settings = {});
    ~AwVkDevice();

    VkDevice getDevice() const { return m_device; }
    const AwVkSettings &getSettings() const { return m_settings; }

  private:
    VkDevice m_device = VK_NULL_HANDLE;
    AwVkSettings m_settings;

    std::vector<std::shared_ptr<AwVkQueue>> m_graphicQueues;
    std::vector<std::shared_ptr<AwVkQueue>> m_presentQueues;
};
} // namespace Airwave