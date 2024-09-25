#include "cube_texture.hpp"
#include "render/renderer/renderer.hpp"
#include "graphics/opengl/gl_texture.hpp"
#include "graphics/opengl/gl_cube_texture.hpp"
namespace Airwave
{
std::shared_ptr<CubeTexture> CubeTexture::Create(const std::string &path, const TextureSpecification &spec)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLCubeTexture>(path, spec);
        case RendererAPI::APIType::Vulkan:
            LOG_ERROR("Vulkan is not supported yet!");
        case RendererAPI::APIType::DirectX:
            LOG_ERROR("DirectX is not supported yet!");
    }

    AW_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

static std::shared_ptr<CubeTexture> Create(uint32_t width, uint32_t height,
                                           const TextureSpecification &spec = TextureSpecification())
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");

            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLCubeTexture>(width, height, spec);
        case RendererAPI::APIType::Vulkan:
            LOG_ERROR("Vulkan is not supported yet!");
        case RendererAPI::APIType::DirectX:
            LOG_ERROR("DirectX is not supported yet!");

        default:
            AW_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
    }
}
} // namespace Airwave
