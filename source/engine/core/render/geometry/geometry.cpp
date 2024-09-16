#include "geometry.hpp"
#include "render/renderer/render_command.hpp"
#include "render/buffer/vertex_buffer.hpp"

namespace Airwave
{
CubeGeometry::CubeGeometry(float width, float height, float depth, int widthSegments,
                           int heightSegments, int depthSegments)
    : m_width(width), m_height(height), m_depth(depth), m_widthSegments(widthSegments),
      m_heightSegments(heightSegments), m_depthSegments(depthSegments)
{
    updateData();
}

void CubeGeometry::updateData()
{
    std::vector<AWVertex> vertices;
    std::vector<uint32_t> indices;
    GeometryUtils::CreateCube(vertices, indices, m_width, m_height, m_depth, m_widthSegments,
                              m_heightSegments, m_depthSegments);

    m_vertices = GeometryUtils::ConvertAWVertexToFloatArray(vertices);
    m_indices  = indices;

    if (!m_vertexArray) m_vertexArray = VertexArray::Create();
    {
        m_vertexArray->bind();
        auto vertexBuffer =
            VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(float));
        vertexBuffer->setBufferLayout({{ShaderDataType::FLOAT3, "a_position"},
                                       {ShaderDataType::FLOAT3, "a_normal"},
                                       {ShaderDataType::FLOAT2, "a_uv"}});
        m_vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(indices.data(), m_indices.size() * sizeof(uint32_t));
        m_vertexArray->setIndexBuffer(indexBuffer);
        m_vertexArray->unbind();
    }
}

void CubeGeometry::draw() const { RenderCommand::DrwaIndexed(m_vertexArray); }

void CubeGeometry::setSize(float width, float height, float depth)
{
    m_width  = width;
    m_height = height;
    m_depth  = depth;
    updateData();
}

void CubeGeometry::setSegments(int widthSegments, int heightSegments, int depthSegments)
{
    m_widthSegments  = widthSegments;
    m_heightSegments = heightSegments;
    m_depthSegments  = depthSegments;
    updateData();
}

SphereGeometry::SphereGeometry(float radius, int widthSegments, int heightSegments, float phiStart,
                               float phiLength, float thetaStart, float thetaLength)

    : m_radius(radius), m_widthSegments(widthSegments), m_heightSegments(heightSegments),
      m_phiStart(phiStart), m_phiLength(phiLength), m_thetaStart(thetaStart),
      m_thetaLength(thetaLength)
{
    updateData();
}

void SphereGeometry::updateData()
{
    std::vector<AWVertex> vertices;
    std::vector<uint32_t> indices;
    GeometryUtils::CreateSphere(vertices, indices, m_radius, m_widthSegments, m_heightSegments,
                                m_phiStart, m_phiLength, m_thetaStart, m_thetaLength);

    m_vertices = GeometryUtils::ConvertAWVertexToFloatArray(vertices);
    m_indices  = indices;

    if (!m_vertexArray) m_vertexArray = VertexArray::Create();
    {
        m_vertexArray->bind();
        auto vertexBuffer =
            VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(float));
        vertexBuffer->setBufferLayout({{ShaderDataType::FLOAT3, "a_position"},
                                       {ShaderDataType::FLOAT3, "a_normal"},
                                       {ShaderDataType::FLOAT2, "a_uv"}});
        m_vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer = IndexBuffer::Create(indices.data(), m_indices.size() * sizeof(uint32_t));
        m_vertexArray->setIndexBuffer(indexBuffer);
        m_vertexArray->unbind();
    }
}

void SphereGeometry::draw() const { RenderCommand::DrwaIndexed(m_vertexArray); }

} // namespace Airwave