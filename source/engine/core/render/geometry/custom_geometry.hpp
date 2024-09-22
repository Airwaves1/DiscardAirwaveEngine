#pragma once

#include "geometry.hpp"
#include "render/renderer/render_command.hpp"

namespace Airwave
{
class CustomGeometry : public Geometry
{
  public:
    CustomGeometry(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = vertices;
        m_indices = indices;
        updateData();
    }

    ~CustomGeometry() override = default;

    void setVertices(const std::vector<AWVertex> &vertices) override
    {
        std::vector<float> data = GeometryUtils::ConvertAWVertexToFloatArray(vertices);
        m_vertices = data;
        updateData();
    }

    void setVertices(const std::vector<float> &vertices) override
    {
        m_vertices = vertices;
        updateData();
    }

    void setIndices(const std::vector<uint32_t> &indices) override
    {
        m_indices = indices;
        updateData();
    }

    void updateData() override
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
            auto vertexBuffer =
                VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(float));
            vertexBuffer->setBufferLayout({
                {ShaderDataType::FLOAT3, "a_position"},
                {ShaderDataType::FLOAT3, "a_normal"},
                {ShaderDataType::FLOAT2, "a_texCoord"},
            });
            m_vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = IndexBuffer::Create(m_indices.data(), m_indices.size());
            m_vertexArray->setIndexBuffer(indexBuffer);
        }
        m_vertexArray->unbind();
    }

    void draw() const override
    {
        if (!m_vertexArray)
        {
            LOG_ERROR("VertexArray must not be null");
            return;
        }

        RenderCommand::DrwaIndexed(m_vertexArray);
    }
};

} // namespace Airwave
