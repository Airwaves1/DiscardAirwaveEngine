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
        
		void bind() const override;
		void unbind() const override;
        void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override;
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override;
    private:
        // 一个VAO可以挖取多个VBO里的数据(VAO存的是引用)
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        unsigned int m_index;
    };

} // namespace Airwave

#endif // !OPENGL_VERTEX_ARRAY_H