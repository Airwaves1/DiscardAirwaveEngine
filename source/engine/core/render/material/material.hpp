#pragma once

#include <variant>
#include "render/shader/shader.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

class Material
{
  public:
    Material() = default;

    virtual void bind() = 0;

    void setTexture(const std::string &name, std::shared_ptr<Texture2D> texture, uint32_t slot)
    {
        m_textures[name] = std::make_pair(texture, slot);
        setUniform("u_textureCount", static_cast<int>(m_textures.size()));
    }

    template <typename T> void setUniform(const std::string &name, const T &value)
    {
        m_uniforms[name] = value;
    }

    virtual void uploadUniforms();

    virtual void setShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
    virtual std::shared_ptr<Shader> getShader() { return m_shader; }

  public:
    void uploadUniform(const std::string &name, int value);
    void uploadUniform(const std::string &name, float value);
    void uploadUniform(const std::string &name, const glm::vec2 &value);
    void uploadUniform(const std::string &name, const glm::vec3 &value);
    void uploadUniform(const std::string &name, const glm::vec4 &value);
    void uploadUniform(const std::string &name, const glm::mat3 &value);
    void uploadUniform(const std::string &name, const glm::mat4 &value);

  protected:
    std::shared_ptr<Shader> m_shader;
    std::unordered_map<std::string, std::pair<std::shared_ptr<Texture2D>, uint32_t>> m_textures;
    std::unordered_map<std::string, UniformValue> m_uniforms;

    bool b_transparent = false;
};

} // namespace Airwave