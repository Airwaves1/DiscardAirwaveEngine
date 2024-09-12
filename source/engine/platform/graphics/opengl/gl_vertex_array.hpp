#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "render/buffer/vertex_array.hpp"

namespace Airwave
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
        
		void Bind() const override;
		void Unbind() const override;
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
    private:
        // 一个VAO可以挖取多个VBO里的数据(VAO存的是引用)
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        unsigned int m_Index;
    };

} // namespace Airwave

#endif // !OPENGL_VERTEX_ARRAY_H