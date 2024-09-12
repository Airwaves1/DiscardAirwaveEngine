#include "vertex_buffer.hpp"
#include "render/renderer/renderer_api.hpp"
#include "graphics/opengl/gl_vertex_buffer.hpp"

#include <memory>
namespace Airwave
{
// 注意声明为static的函数，在定义的时候不需要写static关键字
// 而且这个Create函数是在基类定义的，因为创建的窗口对象应该包含多种平台的派生类对象，所以放到了基类里
// 而且这个基类的cpp引用了相关的派生类的头文件
std::shared_ptr<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
{
    std::shared_ptr<VertexBuffer> buffer = nullptr;
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
            buffer = std::make_shared<OpenGLVertexBuffer>(vertices, size);
            break;
        }

        default:
            AW_ASSERT(false, "Unknown RendererAPI");
            break;
    }

    return buffer;
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    std::shared_ptr<VertexBuffer> buffer = nullptr;
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
            buffer = std::make_shared<OpenGLVertexBuffer>(size);
            break;
        }

        default:
            AW_ASSERT(false, "Unknown RendererAPI");
            break;
    }

    return buffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t *data, uint32_t size)
{
    std::shared_ptr<IndexBuffer> buffer = nullptr;
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
            buffer = std::make_shared<OpenGLIndexBuffer>(data, size);
            break;
        }
        default:
            AW_ASSERT(false, "Unknown RendererAPI");
            break;
    }

    return buffer;
}

void BufferLayout::calculateElementsOffsets()
{
    m_stride = 0;
    for (auto &element : m_elements)
    {
        element.SetOffset(m_stride);
        m_stride += element.getSize();
    }
}

bool BufferElement::isIntergerType() const
{
    if (m_type == ShaderDataType::INT) return true;
    return false;
}

} // namespace Airwave