#include <iostream>

#include "utils/common.hpp"
#include "utils/log.hpp"

#include "window/aw_window.hpp"
#include "graphics/graphic_contex.hpp"
#include "graphics/vulkan/aw_vk_context.hpp"
#include "graphics/vulkan/aw_vk_device.hpp"
#include "graphics/vulkan/aw_vk_swapchain.hpp"
int main()
{
    Airwave::Log::Init();

    std::cout << "Hello, AirwaveEngine!" << std::endl;
    LOG_TRACE("Hello, AirwaveEngine");
    LOG_DEBUG("Hello, AirwaveEngine");
    LOG_INFO("Hello, AirwaveEngine");
    LOG_WARN("Hello, AirwaveEngine");
    LOG_ERROR("Hello, AirwaveEngine");

    auto window = Airwave::AwWindow::Create(800, 600, "AirwaveEngine");
    auto graphicContext = Airwave::GraphicContext::Create(window.get());
    auto vkContext = dynamic_cast<Airwave::AwVkContext *>(graphicContext.get());
    auto vkDevice  = std::make_shared<Airwave::AwVkDevice>(vkContext, 1, 1);
    auto vkSwapchain = std::make_shared<Airwave::AwVkSwapchain>(vkContext, vkDevice.get());
    
    vkSwapchain->recreate();

    while (!window->shouldClose())
    {
        window->pollEvents();
        window->swapBuffers();
    }

    return 0;
}