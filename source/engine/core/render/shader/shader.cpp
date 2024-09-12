#include "shader.hpp"
#include "utils/common.hpp"

#include "render/renderer/renderer.hpp"
#include "graphics/opengl/gl_shader.hpp"

namespace Airwave
{

    Shader* Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc, bool fromFile)
    {
        RendererAPI::APIType type = Renderer::GetAPI();
        switch (type)
        {
        case RendererAPI::APIType::None:
            LOG_ERROR("RendererAPI::None is not supported");
            return nullptr;
        case RendererAPI::APIType::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc, fromFile);
        default:
            LOG_ERROR("Unknown RendererAPI");
            return nullptr;
        }

        return nullptr;
    }
} // namespace Airwave