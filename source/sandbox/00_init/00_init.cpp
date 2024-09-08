#include <iostream>

#include "utils/common.hpp"
#include "utils/log.hpp"

#include "window/aw_window.hpp"

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

    while (!window->shouldClose())
    {
        window->pollEvents();
        window->swapBuffers();
    }

    return 0;
}