#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "render/shader/shader.hpp"

namespace Airwave
{

class OpenGLShader : public Shader
{
  public:
    OpenGLShader(const std::string &vertex, const std::string &fragment, bool fromFile = true);

    ~OpenGLShader();

    void bind() const override;
    void unbind() const override;

    void uploadUniformInt(const std::string &name, int value) override;
    void uploadUniformFloat(const std::string &name, float value) override;
    void uploadUniformFloat2(const std::string &name, const glm::vec2 &vector2) override;
    void uploadUniformFloat3(const std::string &name, const glm::vec3 &vector3) override;
    void uploadUniformFloat4(const std::string &name, const glm::vec4 &vector4) override;
    void uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) override;
    void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) override;

    void createDownScaleFramebuffer() {}
    void drawDownScaleFramebuffer(uint32_t MSAAbuffer, uint32_t buffer, uint32_t width,
                                  uint32_t height)
    {
    }

  private:
    void compileOrGetOpenGLBinaries(); // 编译或获取OpenGL二进制文件
    void compileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>
                                        &shaderSources); // 编译或获取Vulkan二进制文件
    void reflect(ShaderType stage, const std::vector<uint32_t> &shaderData);

  private:
    // m_VulkanSPIRVCache相当于一个缓存的unordered_map, key是子着色器类型
    // value是一个uint32的数组, 代表着Spir-V对应的编译结果
    std::unordered_map<ShaderType, std::vector<uint32_t>> m_vulkanSPIRVCache;
    std::unordered_map<ShaderType, std::vector<uint32_t>> m_openGLSPIRVCache;

    std::unordered_map<ShaderType, std::string> m_openGLSourceCode;

    std::string m_filePath;

    int m_programIdBeforeBind = -1;

    std::unordered_map<std::string, bool> m_uniformErrordCache;

  public:
    // TODO: To be refactered
    uint32_t resolveFBO;
    uint32_t quadVAO, quadVBO;
    uint32_t screenTexture;
    uint32_t instanceIdTexture;

    int m_tempProgramId;
};

} // namespace Airwave

#endif // OPENGL_SHADER_H