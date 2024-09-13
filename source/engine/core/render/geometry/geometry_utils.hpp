#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include <glm/glm.hpp>
#include <vector>

const double PI = 3.14159265358979323846;

namespace Airwave
{
    struct AWVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };

    class GeometryUtils
    {
    public:
        // 函数将顶点数据转换为扁平化的 float 数组
        static std::vector<float> ConvertAWVertexToFloatArray(const std::vector<Airwave::AWVertex> &vertices)
        {
            std::vector<float> result;
            result.reserve(vertices.size() * 8); // 每个顶点包含5个浮点数

            for (const auto &vertex : vertices)
            {
                // 将位置、纹理坐标和法线数据转换为 float 数组
                result.push_back(vertex.position.x);
                result.push_back(vertex.position.y);
                result.push_back(vertex.position.z);

                result.push_back(vertex.texCoord.x);
                result.push_back(vertex.texCoord.y);

                result.push_back(vertex.normal.x);
                result.push_back(vertex.normal.y);
                result.push_back(vertex.normal.z);
            }

            return result;
        }

    public:
        /**
         * Plane
         *
         * width: 1.0f
         * height: 1.0f
         * widthSegments: 1
         * heightSegments: 1
         */
        static void CreatePlane(std::vector<AWVertex> &vertices,
                                std::vector<uint32_t> &indices,
                                float width = 1.0f,
                                float height = 1.0f,
                                int widthSegments = 1,
                                int heightSegments = 1);

        /**
         * Cube
         *
         * width: 1.0f
         * height: 1.0f
         * depth: 1.0f
         * widthSegments: 1
         * heightSegments: 1
         * depthSegments: 1
         */
        static void CreateCube(std::vector<AWVertex> &vertices,
                               std::vector<uint32_t> &indices,
                               float width = 1.0f,
                               float height = 1.0f,
                               float depth = 1.0f,
                               int widthSegments = 1,
                               int heightSegments = 1,
                               int depthSegments = 1);

        /**
         * Sphere
         *
         * radius: 1.0f
         * widthSegments: 32
         * heightSegments: 32
         * phiStart: 0.0f
         * phiLength: 2 * PI
         * thetaStart: 0.0f
         * thetaLength: PI
         */
        static void CreateSphere(std::vector<AWVertex> &vertices,
                                 std::vector<uint32_t> &indices,
                                 float radius = 1.0f,
                                 int widthSegments = 32,
                                 int heightSegments = 32,
                                 float phiStart = 0.0f,
                                 float phiLength = 2 * PI,
                                 float thetaStart = 0.0f,
                                 float thetaLength = PI);
    };

} // namespace Airwave

#endif // !GEOMETRY_UTILS_H