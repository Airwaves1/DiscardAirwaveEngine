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

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual std::string GetName() const { return m_Name; }

    virtual void UploadUniformInt(const std::string &name, int value) = 0;

    virtual void UploadUniformFloat(const std::string &name, float value)               = 0;
    virtual void UploadUniformFloat2(const std::string &name, const glm::vec2 &vector2) = 0;
    virtual void UploadUniformFloat3(const std::string &name, const glm::vec3 &vector3) = 0;
    virtual void UploadUniformFloat4(const std::string &name, const glm::vec4 &vector4) = 0;

    virtual void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) = 0;
    virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) = 0;

  public:
    static Shader *Create(const std::string &vertexSrc, const std::string &fragmentSrc,
                          bool fromFile = true);

  protected:
    uint32_t m_RendererID;
    std::string m_Name;
};

} // namespace Airwave
