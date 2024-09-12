#include "gl_vertex_array.hpp"

#include "utils/common.hpp"
#include <glad/glad.h>
#include "render/buffer/vertex_buffer.hpp"

namespace Airwave
{

    static GLenum GetShaderDataTypeToOpenGL(const ShaderDataType &type)
    {
        switch (type)
        {
        case ShaderDataType::FLOAT:
            return GL_FLOAT;
        case ShaderDataType::FLOAT2:
            return GL_FLOAT;
        case ShaderDataType::FLOAT3:
            return GL_FLOAT;
        case ShaderDataType::FLOAT4:
            return GL_FLOAT;
        case ShaderDataType::INT:
            return GL_INT;
        case ShaderDataType::INT2:
            return GL_INT;
        case ShaderDataType::INT3:
            return GL_INT;
        case ShaderDataType::INT4:
            return GL_INT;
        case ShaderDataType::MAT3:
            return GL_FLOAT;
        case ShaderDataType::MAT4:
            return GL_FLOAT;
        case ShaderDataType::BOOL:
            return GL_BOOL;
        }
        AW_ASSERT(false, "Unknown ShaderDataType")
        return GL_FALSE;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_Index);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_Index);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_Index);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
    {
        AW_ASSERT(vertexBuffer->GetBufferLayout().GetCount(), "Empty Layout in VertexBuffer!");

        glBindVertexArray(m_Index);
        vertexBuffer->Bind();

        BufferLayout layout = vertexBuffer->GetBufferLayout();
        int index = 0;
        for (const BufferElement &element : layout)
        {
            glEnableVertexAttribArray(index);
            if (element.IsIntergerType())
            {
                glVertexAttribIPointer(index,
                                       GetShaderTypeDataCount(element.GetType()),
                                       GetShaderDataTypeToOpenGL(element.GetType()),
                                       layout.GetStride(),
                                       (const void *)(uint64_t)(element.GetOffset()));
            }
            else
            {
                glVertexAttribPointer(index,
                                      GetShaderTypeDataCount(element.GetType()),
                                      GetShaderDataTypeToOpenGL(element.GetType()),
                                      element.IsNormalized() ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (const void *)(uint64_t)(element.GetOffset()));
                                
            }
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_Index);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    const std::vector<std::shared_ptr<VertexBuffer>> &OpenGLVertexArray::GetVertexBuffers() const
    {
        return m_VertexBuffers;
    }

    const std::shared_ptr<IndexBuffer> &OpenGLVertexArray::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }

} // namespace Airwave