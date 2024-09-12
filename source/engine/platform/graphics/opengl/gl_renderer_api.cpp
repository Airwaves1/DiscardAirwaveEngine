#include "gl_renderer_api.hpp"
#include <glad/glad.h>
#include "utils/log.hpp"

// 真正直接调用OpenGL函数的地方

namespace Airwave
{
    void OpenGLRendererAPI::Init() const
    {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LESS);
        glEnable(GL_MULTISAMPLE);
    }

    void OpenGLRendererAPI::Clear(RenderClearFlag flag) const
    {
        uint32_t clearFlag = 0;
        if ((flag & RenderClearFlag::Color) == RenderClearFlag::Color)
            clearFlag |= GL_COLOR_BUFFER_BIT;
        if ((flag & RenderClearFlag::Depth) == RenderClearFlag::Depth)
            clearFlag |= GL_DEPTH_BUFFER_BIT;
        if ((flag & RenderClearFlag::Stencil) == RenderClearFlag::Stencil)
            clearFlag |= GL_STENCIL_BUFFER_BIT;

        glClear(clearFlag);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        if (m_ClearColor != color)
        {
            m_ClearColor = color;
            glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        }
    }

    void OpenGLRendererAPI::Enable(RenderState state)
    {
        // 计算要启用的状态位
        RenderState statesToEnable = state ^ m_CurRenderState;

        // 更新状态
        for (int i = 0; i < 8; i++)
        {
            if ((static_cast<uint8_t>(statesToEnable) >> i) & 1)
            {
                glEnable(RenderStateToGLenum(static_cast<RenderState>(1 << i)));
            }
        }
    }

    void OpenGLRendererAPI::Disable(RenderState state)
    {
        // 计算要禁用的状态位
        RenderState statesToDisable = m_CurRenderState ^ state;

        // 更新状态
        for (int i = 0; i < 8; i++)
        {
            if ((static_cast<uint8_t>(statesToDisable) >> i) & 1)
            {
                glDisable(RenderStateToGLenum(static_cast<RenderState>(1 << i)));
            }
        }
    }

    void OpenGLRendererAPI::OnViewportResize(uint32_t width, uint32_t height) const
    {
        glViewport(0, 0, width, height);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &va, uint32_t count) const
    {

        if (count == 0)
        {
            glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
            LOG_DEBUG("va->GetIndexBuffer()->GetCount() : {0}", va->GetIndexBuffer()->GetCount());
        }
        else
        {
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }
    }

    GLenum OpenGLRendererAPI::RenderStateToGLenum(RenderState state)
    {
        switch (state)
        {
        case RenderState::DepthTest:
            return GL_DEPTH_TEST;
        case RenderState::Blend:
            return GL_BLEND;
        case RenderState::CullFace:
            return GL_CULL_FACE;
        case RenderState::StencilTest:
            return GL_STENCIL_TEST;
        case RenderState::ScissorTest:
            return GL_SCISSOR_TEST;
        case RenderState::Multisample:
            return GL_MULTISAMPLE;
        default:
            return 0;
        }
    }

}
