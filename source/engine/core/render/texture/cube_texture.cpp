#include "cube_texture.hpp"
#include "render/renderer/renderer.hpp"
#include "graphics/opengl/gl_texture.hpp"
#include "graphics/opengl/gl_cube_texture.hpp"
namespace Airwave
{
std::shared_ptr<CubeTexture> CubeTexture::Create(const std::array<std::string, 6>& faces)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLCubeTexture>(faces);
        case RendererAPI::APIType::Vulkan:
            LOG_ERROR("Vulkan is not supported yet!");
        case RendererAPI::APIType::DirectX:
            LOG_ERROR("DirectX is not supported yet!");
    }

    AW_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}


} // namespace Airwave
