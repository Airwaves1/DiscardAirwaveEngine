#pragma once

#include "render/geometry/geometry_utils.hpp"
#include "render/buffer/vertex_array.hpp"
#include "render/renderer/render_command.hpp"
#include "ecs/aw_component.hpp"
namespace Airwave
{
class GeometryComponent
{
  public:
    GeometryComponent() = default;

    GeometryComponent(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = vertices;
        m_indices  = indices;
        updateData();
    }

    ~GeometryComponent() = default;

    void setVertices(const std::vector<AWVertex> &vertices)
    {
        std::vector<float> data = GeometryUtils::ConvertAWVertexToFloatArray(vertices);
        m_vertices              = data;
        updateData();
    }

    void setVertices(const std::vector<float> &vertices)
    {
        m_vertices = vertices;
        updateData();
    }

    void setIndices(const std::vector<uint32_t> &indices)
    {
        m_indices = indices;
        updateData();
    }

    void setData(const std::vector<AWVertex> &vertices, const std::vector<uint32_t> &indices)
    {
        std::vector<float> data = GeometryUtils::ConvertAWVertexToFloatArray(vertices);
        m_vertices              = data;
        m_indices               = indices;
        updateData();
    }

    void setData(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = vertices;
        m_indices  = indices;
        updateData();
    }

    void updateData()
    {
        if (m_vertices.empty())
        {
            LOG_ERROR("Vertices must not be empty");
            return;
        }

        if (!m_vertexArray)
        {
            m_vertexArray = VertexArray::Create();
        }

        m_vertexArray->bind();
        {
            auto vertexBuffer = VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(float));
            vertexBuffer->setBufferLayout({
                {ShaderDataType::FLOAT3, "a_position"},
                {ShaderDataType::FLOAT3, "a_normal"},
                {ShaderDataType::FLOAT2, "a_texCoord"},
            });
            m_vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = IndexBuffer::Create(m_indices.data(), m_indices.size() * sizeof(uint32_t));
            m_vertexArray->setIndexBuffer(indexBuffer);
        }
        m_vertexArray->unbind();
    }

    void draw() const
    {
        if (!m_vertexArray)
        {
            LOG_ERROR("VertexArray must not be null");
            return;
        }

        RenderCommand::DrwaIndexed(m_vertexArray);
    }

    uint32_t getIndicesCount() const { return m_indices.size(); }
    uint32_t getVerticesCount() const { return m_vertices.size(); }

  private:
    std::vector<float> m_vertices;
    std::vector<uint32_t> m_indices;
    std::shared_ptr<VertexArray> m_vertexArray;
};

} // namespace Airwave
