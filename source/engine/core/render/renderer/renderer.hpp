#pragma once

#include <glm/glm.hpp>
#include "render/buffer/vertex_array.hpp"
#include "utils/common.hpp"
#include "render/shader/shader.hpp"
#include "render/renderer/renderer_api.hpp"
#include "camera/camera.hpp"

// 进行场景渲染，包括渲染的初始化，渲染的开始和结束，以及提交渲染的物体

namespace Airwave
{
    class Renderer
    {
    public:

        // 这个函数真的有必要放到Renderer类里吗, Renderer2D类也要放?
		static inline RendererAPI::APIType GetAPI() { return RendererAPI::GetAPIType(); }

        static void Init();
        static void BeginScene(std::shared_ptr<Camera> camera);
        static void EndScene();

        // 绑定对应的VertexArray, 然后调用DrawCall
        static void Submit(const std::shared_ptr<Shader> &shader,
                           const std::shared_ptr<VertexArray> &vertexArray,
                           const glm::mat4 &transform = glm::mat4(1.0f));

        static void OnViewportResize(uint32_t width, uint32_t height);

    private:
        // 对于不同的物体, MVP矩阵里的M都是不同的, 但是VP矩阵都是相同的
        // 所以这里归类在场景信息里
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 ProjectionMatrix;
        };

        static SceneData *s_SceneData;
    };
}
