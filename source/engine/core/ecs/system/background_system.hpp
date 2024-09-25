#pragma once

#include "ecs/system/system.hpp"
#include "render/texture/cube_texture.hpp"
#include "render/texture/texture_2d.hpp"
#include "render/renderer/render_command.hpp"

#include <glm/glm.hpp>
#include "render/shader/shader.hpp"
#include "render/shader/shader_library.hpp"
#include "render/buffer/vertex_array.hpp"
#include "render/buffer/vertex_buffer.hpp"
#include "utils/file_utils.hpp"
#include <glad/glad.h>
namespace Airwave
{

enum class BackgroundType
{
    Color,
    CubeTexture,
    Skybox
};

class BackgroundSystem : public System
{
  public:
    BackgroundSystem(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
        : m_type(BackgroundType::Color), m_color(color)
    {
        setBackgroud(color);
    }

    BackgroundSystem(const std::shared_ptr<CubeTexture> &cubeTexture)
        : m_type(BackgroundType::CubeTexture), m_cubeTexture(cubeTexture)
    {
        setBackgroud(cubeTexture);
    }

    BackgroundSystem(const std::shared_ptr<Texture2D> &skyboxTexture)
        : m_type(BackgroundType::Skybox), m_skyboxTexture(skyboxTexture)
    {
        setBackgroud(skyboxTexture);
    }

    virtual ~BackgroundSystem() {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) {}

    void setBackgroud(const glm::vec4 &color)
    {
        m_type  = BackgroundType::Color;
        m_color = color;
    }

    void setBackgroud(const std::shared_ptr<CubeTexture> &cubeTexture)
    {
        m_type        = BackgroundType::CubeTexture;
        m_cubeTexture = cubeTexture;
    }

    void setBackgroud(const std::shared_ptr<Texture2D> &skyboxTexture)
    {
        m_type          = BackgroundType::Skybox;
        m_skyboxTexture = skyboxTexture;
    }

    void render(std::shared_ptr<Camera> camera)
    {
        if (m_vertexArray == nullptr)
        {
            m_vertexArray = VertexArray::Create();

            std::vector<float> vertices   = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                                             1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                                             -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                                             -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                                             1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                                             1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                                             -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                                             1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                                             -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                                             1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                                             -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                                             1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
            std::vector<uint32_t> indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                             12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

            auto vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
            {
                BufferLayout layout = {
                    {ShaderDataType::FLOAT3, "a_Position"},
                };
                vertexBuffer->setBufferLayout(layout);
                m_vertexArray->addVertexBuffer(vertexBuffer);
            }
            auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
            m_vertexArray->setIndexBuffer(indexBuffer);

            m_shader = SHADER_LIB.load("background", SHADER_DIR "shader_lib/cube_texture_background.vert",
                                       SHADER_DIR "shader_lib/cube_texture_background.frag");
        }

        m_shader->bind();
        m_shader->uploadUniformMat4("u_viewMatrix", glm::mat4(glm::mat3(camera->getViewMatrix())));
        m_shader->uploadUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
        glDepthFunc(GL_LEQUAL);
        switch (m_type)
        {
            case BackgroundType::Color:
                RenderCommand::SetClearColor(m_color);
                RenderCommand::Clear(RenderClearFlag::Color | RenderClearFlag::Depth);
                break;
            case BackgroundType::CubeTexture:
                m_cubeTexture->bind();
                m_vertexArray->bind();
                // RenderCommand::DrawIndexed(m_vertexArray);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                break;
            case BackgroundType::Skybox:
                m_skyboxTexture->bind();
                m_vertexArray->bind();
                RenderCommand::DrawIndexed(m_vertexArray);
                break;
            default:
                break;
        }
        glDepthFunc(GL_LESS);
    }

  private:
    BackgroundType m_type;
    glm::vec4 m_color;
    std::shared_ptr<CubeTexture> m_cubeTexture;
    std::shared_ptr<Texture2D> m_skyboxTexture;

    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Shader> m_shader;
};

} // namespace Airwave
