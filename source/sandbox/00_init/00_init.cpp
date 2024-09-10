#include <iostream>

#include "utils/common.hpp"
#include "utils/log.hpp"

#include "window/aw_window.hpp"
#include "graphics/graphic_contex.hpp"
#include "graphics/vulkan/aw_vk_context.hpp"
#include "graphics/vulkan/aw_vk_device.hpp"

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
    auto context = Airwave::GraphicContext::Create(window.get());
    std::shared_ptr<Airwave::AwVkDevice> device = std::make_shared<Airwave::AwVkDevice>(dynamic_cast<Airwave::AwVkContext*>(context.get()), 1, 1);
    while (!window->shouldClose())
    {
        window->pollEvents();
        window->swapBuffers();
    }

    return 0;
}