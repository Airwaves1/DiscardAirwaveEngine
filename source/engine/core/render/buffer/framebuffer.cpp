#include "framebuffer.hpp"
#include "render/renderer/renderer_api.hpp"
#include "graphics/opengl/gl_framebuffer.hpp"

namespace Airwave
{

std::shared_ptr<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height,
                                                 const FramebufferSpecification &spec)
{
    switch (RendererAPI::GetAPIType())
    {
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLFramebuffer>(width, height, spec);

        case RendererAPI::APIType::Vulkan:
            AW_ASSERT(false, "RendererAPI::Vulkan is currently not supported!");
            // return std::make_shared<VulkanFramebuffer>(width, height, spec);
            return nullptr;
        case RendererAPI::APIType::DirectX:
            AW_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            // return std::make_shared<DirectXFramebuffer>(width, height, spec);
            return nullptr;

        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
    }
}

} // namespace Airwave
