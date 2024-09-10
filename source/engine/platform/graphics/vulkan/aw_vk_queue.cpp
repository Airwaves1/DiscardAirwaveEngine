#include "graphics/vulkan/aw_vk_queue.hpp"

namespace Airwave
{
AwVkQueue::AwVkQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue queue,
                     bool isPresentQueue)
    : m_queueFamilyIndex(queueFamilyIndex), m_queueIndex(queueIndex), m_queue(queue),
      m_isPresentQueue(isPresentQueue)
{
    LOG_TRACE("create queue family index: {0}, queue index: {1}, is present queue: {2}",
              queueFamilyIndex, queueIndex, isPresentQueue);
}

AwVkQueue::~AwVkQueue()
{

}

void AwVkQueue::waitIdle() const { CALL_VK(vkQueueWaitIdle(m_queue)); }

} // namespace Airwave
