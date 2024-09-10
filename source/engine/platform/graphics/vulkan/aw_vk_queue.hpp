#pragma once

#include "graphics/vulkan/aw_vk_common.hpp"

namespace Airwave
{
class AwVkQueue
{
  public:
    AwVkQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue queue, bool isPresentQueue);
    ~AwVkQueue();

    void waitIdle() const;
    VkQueue getQueue() const;

  private:
    uint32_t m_queueFamilyIndex;
    uint32_t m_queueIndex;
    VkQueue m_queue;
    bool m_isPresentQueue;
};

} // namespace Airwave
