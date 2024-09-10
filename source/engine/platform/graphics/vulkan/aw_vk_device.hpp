#pragma once

#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{

struct AwVkSettings
{
    uint32_t graphicQueueCount;
    uint32_t presentQueueCount;
    const char *deviceExtensions;
};

class AwVkQueue;
class AwVkContext;
class AwVkDevice
{
  public:
    AwVkDevice(AwVkContext *context, uint32_t graphicQueueCount, uint32_t presentQueueCount,
               const AwVkSettings &settings = {});
    ~AwVkDevice();

  private:
    VkDevice m_device = VK_NULL_HANDLE;

    std::vector<std::shared_ptr<AwVkQueue>> m_graphicQueues;
    std::vector<std::shared_ptr<AwVkQueue>> m_presentQueues;
};
} // namespace Airwave