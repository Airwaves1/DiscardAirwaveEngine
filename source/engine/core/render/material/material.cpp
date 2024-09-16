#include "material.hpp"
#include "utils/log.hpp"
namespace Airwave
{
    void Material::setTexture(const std::string &name, std::shared_ptr<Texture2D> texture)
    {
        m_textures[name] = texture;
    }

void Material::uploadUniforms()
{
    for (const auto &[name, value] : m_uniforms)
    {
        std::visit([&](auto &&arg) { uploadUniform(name, arg); }, value);
    }
}

void Material::uploadUniform(const std::string &name, int value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformInt(name, value);
}

void Material::uploadUniform(const std::string &name, float value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformFloat(name, value);
}

void Material::uploadUniform(const std::string &name, const glm::vec2 &value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformFloat2(name, value);
}

void Material::uploadUniform(const std::string &name, const glm::vec3 &value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformFloat3(name, value);
}

void Material::uploadUniform(const std::string &name, const glm::vec4 &value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformFloat4(name, value);
}

void Material::uploadUniform(const std::string &name, const glm::mat3 &value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformMat3(name, value);
}

void Material::uploadUniform(const std::string &name, const glm::mat4 &value)
{
    AW_ASSERT(m_shader, "Shader is not set!");
    m_shader->uploadUniformMat4(name, value);
}

} // namespace Airwave
