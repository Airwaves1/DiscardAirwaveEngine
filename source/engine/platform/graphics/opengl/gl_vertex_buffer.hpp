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

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(uint32_t pos, void *data, uint32_t len) override;

        virtual BufferLayout &GetBufferLayout() override { return m_Layout; }
        virtual void SetBufferLayout(const BufferLayout &layout) override { m_Layout = layout; }

    private:
        uint32_t m_VertexBuffer;
        BufferLayout m_Layout;
    };

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);	// Static
		virtual ~OpenGLIndexBuffer() override;

		uint32_t GetCount() const override;

		void Bind()const override;
		void Unbind() const override;

	private:
		uint32_t m_Count;
		uint32_t m_IndexBuffer;
	};

} // namespace Airwave

#endif // OPENGL_BUFFER_H