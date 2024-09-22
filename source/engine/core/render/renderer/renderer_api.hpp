#pragma once

#include <glm/glm.hpp>
#include "render/buffer/vertex_array.hpp"
#include "render/renderer/renderer_state.hpp"

// 这个类是一个抽象类，用于封装不同的渲染API，比如OpenGL, DirectX等，为渲染器提供统一的接口

namespace Airwave
{

    class RendererAPI
    {
    public:
        enum class APIType
        {
            None = 0,
            OpenGL = 1,
            DirectX = 2,
            Vulkan = 3,
        };

    public:
        // 把相关代码抽象成以下三个接口，放在RenderAPI类里, 这些都是抽象接口, 具体会通过创建一个与平台相关的子类的
        // RendererAPI的静态对象, 比如OpenGLRenderer, 然后把RenderCommand作为一个Wrapper, 将其接口暴露出来
        virtual void init() const = 0;

        virtual void clear(RenderClearFlag flag = RenderClearFlag::Color | RenderClearFlag::Depth | RenderClearFlag::Stencil) const = 0;
        virtual void setClearColor(const glm::vec4 &) = 0;

        virtual void enable(RenderState state)  = 0;
        virtual void disable(RenderState state)  = 0;

        virtual void setPolygonMode(RenderPolygonMode mode) const = 0;

        virtual void onViewportResize(uint32_t width, uint32_t height) const = 0;
        virtual void drawIndexed(const std::shared_ptr<VertexArray> &, uint32_t count) const = 0; // count为0则绘制整个IndexBuffer

        inline static APIType GetAPIType() { return s_curType; }
        inline static void SetAPIType(APIType type) { s_curType = type; }

    protected:
        static APIType s_curType;

        RenderState m_curRenderState;
        RenderClearFlag m_curClearFlag;
        glm::vec4 m_clearColor;
    };

} // namespace Airwave
