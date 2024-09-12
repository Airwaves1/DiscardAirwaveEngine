#pragma once

#include "utils/common.hpp"
#include <glm/glm.hpp>
#include <string>

namespace Airwave
{
enum class ShaderType
{
    Invalid = -1,
    Vertex,
    Fragment,
    Compute,
    Geometry
};

class Shader
{
  public:
    virtual ~Shader() {}

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual std::string getName() const { return m_name; }

    virtual void uploadUniformInt(const std::string &name, int value) = 0;

    virtual void uploadUniformFloat(const std::string &name, float value)               = 0;
    virtual void uploadUniformFloat2(const std::string &name, const glm::vec2 &vector2) = 0;
    virtual void uploadUniformFloat3(const std::string &name, const glm::vec3 &vector3) = 0;
    virtual void uploadUniformFloat4(const std::string &name, const glm::vec4 &vector4) = 0;

    virtual void uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) = 0;
    virtual void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) = 0;

  public:
    static Shader *Create(const std::string &vertexSrc, const std::string &fragmentSrc,
                          bool fromFile = true);

  protected:
    uint32_t m_rendererID;
    std::string m_name;
};

} // namespace Airwave
