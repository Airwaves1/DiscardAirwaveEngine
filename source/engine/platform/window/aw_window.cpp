#include "window/aw_window.hpp"

#include "utils/log.hpp"
#include "utils/common.hpp"

#include "aw_glfw_window.hpp"

namespace Airwave
{

std::unique_ptr<AwWindow> AwWindow::Create(uint32_t width, uint32_t height, std::string title)
{
#ifdef AW_PLATFORM_WINDOWS
    return std::make_unique<AwGLFWwindow>(width, height, title);
#elif AW_PLATFORM_LINUX
    return std::make_unique<AwGLFWwindow>(width, height, title);
#elif AW_PLATFORM_MACOS
    return std::make_unique<AwGLFWwindow>(width, height, title);
#else
    LOG_ERROR("Unknown Platform");
    return nullptr;
#endif
}

} // namespace Airwave
