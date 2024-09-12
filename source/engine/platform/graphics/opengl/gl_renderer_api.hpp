#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "render/renderer/renderer_api.hpp"
#include <glad/glad.h>

namespace Airwave
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() const override;

        virtual void Clear(RenderClearFlag flag = RenderClearFlag::Color | RenderClearFlag::Depth | RenderClearFlag::Stencil) const override;

        virtual void SetClearColor(const glm::vec4 &color) override;

        virtual void Enable(RenderState state) override;
        virtual void Disable(RenderState state) override;

        virtual void OnViewportResize(uint32_t width, uint32_t height) const override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray> &va, uint32_t count = 0) const override;

    private:
        GLenum RenderStateToGLenum(RenderState state);
        
    };
} // namespace Airwave

#endif // OPENGL_RENDERER_API_H