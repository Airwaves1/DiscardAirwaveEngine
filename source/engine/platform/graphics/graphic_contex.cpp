#include "graphic_contex.hpp"

#include "utils/common.hpp"

#include "graphics/vulkan/aw_vk_context.hpp"
#include "graphics/opengl/aw_gl_contex.hpp"

namespace Airwave
{
    
std::unique_ptr<GraphicContext> GraphicContext::Create(AwWindow *window)
{
#ifdef AW_ENGINE_GRAPHICS_API_VULKAN
    return std::make_unique<AwVkContext>(window);
#endif

#ifdef AW_ENGINE_GRAPHICS_API_OPENGL
    return std::make_unique<AwGLContext>(window);
#endif

}

}