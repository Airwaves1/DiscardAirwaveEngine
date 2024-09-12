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
        glGenVertexArrays(1, &m_index);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_index);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_index);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
    {
        AW_ASSERT(vertexBuffer->getBufferLayout().getCount(), "Empty Layout in VertexBuffer!");

        glBindVertexArray(m_index);
        vertexBuffer->bind();

        BufferLayout layout = vertexBuffer->getBufferLayout();
        int index = 0;
        for (const BufferElement &element : layout)
        {
            glEnableVertexAttribArray(index);
            if (element.isIntergerType())
            {
                glVertexAttribIPointer(index,
                                       GetShaderTypeDataCount(element.getType()),
                                       GetShaderDataTypeToOpenGL(element.getType()),
                                       layout.getStride(),
                                       (const void *)(uint64_t)(element.getOffset()));
            }
            else
            {
                glVertexAttribPointer(index,
                                      GetShaderTypeDataCount(element.getType()),
                                      GetShaderDataTypeToOpenGL(element.getType()),
                                      element.isNormalized() ? GL_TRUE : GL_FALSE,
                                      layout.getStride(),
                                      (const void *)(uint64_t)(element.getOffset()));
                                
            }
            index++;
        }
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_index);
        indexBuffer->bind();
        m_indexBuffer = indexBuffer;
    }

    const std::vector<std::shared_ptr<VertexBuffer>> &OpenGLVertexArray::getVertexBuffers() const
    {
        return m_vertexBuffers;
    }

    const std::shared_ptr<IndexBuffer> &OpenGLVertexArray::getIndexBuffer() const
    {
        return m_indexBuffer;
    }

} // namespace Airwave