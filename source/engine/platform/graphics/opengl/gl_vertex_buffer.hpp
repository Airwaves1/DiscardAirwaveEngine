#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "render/buffer/vertex_buffer.hpp"
#include "utils/common.hpp"
#include <glad/glad.h>

namespace Airwave
{

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float *vertices, uint32_t size); // static buffer
        OpenGLVertexBuffer(uint32_t size);                  // dynamic buffer
        virtual ~OpenGLVertexBuffer() override;

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void setData(uint32_t pos, void *data, uint32_t len) override;

        virtual BufferLayout &getBufferLayout() override { return m_layout; }
        virtual void setBufferLayout(const BufferLayout &layout) override { m_layout = layout; }

    private:
        uint32_t m_vertexBuffer;
        BufferLayout m_layout;
    };

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);	// Static
		virtual ~OpenGLIndexBuffer() override;

		uint32_t getCount() const override;

		void bind()const override;
		void unbind() const override;

	private:
		uint32_t m_count;
		uint32_t m_indexBuffer;
	};

} // namespace Airwave

#endif // OPENGL_BUFFER_H