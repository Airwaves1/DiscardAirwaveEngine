#include "vertex_array.hpp"
#include "graphics/opengl/gl_vertex_array.hpp"
#include "render/renderer/renderer_api.hpp"
#include <glad/glad.h>

namespace Airwave
{

    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        std::shared_ptr<VertexArray> buffer = nullptr;
        switch (RendererAPI::GetAPIType())
        {
        case RendererAPI::APIType::None:
        {
            LOG_ERROR("RendererAPI::None is currently not supported!");
            AW_ASSERT(false, "RendererAPI::None is currently not supported!");
            break;
        }
        case RendererAPI::APIType::OpenGL:
        {
            buffer = std::make_shared<OpenGLVertexArray>();
            break;
        }
        default:
            LOG_ERROR("Unknown RendererAPI");
            AW_ASSERT(false, "Unknown RendererAPI");
            break;
        }

        return buffer;
    }

} // namespace Airwave