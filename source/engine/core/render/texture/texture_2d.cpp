#include "texture_2d.hpp"
#include "render/renderer/renderer.hpp"
#include "graphics/opengl/gl_texture.hpp"
namespace Airwave
{
std::shared_ptr<Texture2D> Texture2D::Create(const std::string &path,
                                             const TextureSpecification &spec)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path, spec);
    }

    AW_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height,
                                             const TextureSpecification &spec)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::APIType::None:
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height, spec);
    }

    AW_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Airwave