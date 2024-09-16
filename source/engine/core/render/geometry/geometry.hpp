#pragma once

#include <vector>
#include <memory>
#include "render/buffer/vertex_array.hpp"

namespace Airwave
{
class Geometry
{
  public:
    Geometry()          = default;
    virtual ~Geometry() = default;

    virtual void updateData() = 0;

    virtual void setVertices(const std::vector<float> &vertices) {}
    virtual void setIndices(const std::vector<uint32_t> &indices) {}

    // 绘制几何体
    virtual void draw() const = 0;

    // 获取顶点数据
    std::shared_ptr<VertexArray> getVertexArray() const { return m_vertexArray; }

  protected:
    std::shared_ptr<VertexArray> m_vertexArray;
    std::vector<float> m_vertices;   // 顶点数据
    std::vector<uint32_t> m_indices; // 索引数据
};

class CubeGeometry : public Geometry
{
  public:
    CubeGeometry(float width = 1.0f, float height = 1.0f, float depth = 1.0f, int widthSegments = 1,
                 int heightSegments = 1, int depthSegments  = 1);
    ~CubeGeometry() override = default;

    void updateData() override;

    void draw() const override;

    void setSize(float width, float height, float depth);
    void setSegments(int widthSegments, int heightSegments, int depthSegments);

    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    float getDepth() const { return m_depth; }
    int getWidthSegments() const { return m_widthSegments; }
    int getHeightSegments() const { return m_heightSegments; }
    int getDepthSegments() const { return m_depthSegments; }

  private:
    float m_width;
    float m_height;
    float m_depth;
    int m_widthSegments;
    int m_heightSegments;
    int m_depthSegments;
};

} // namespace Airwave
